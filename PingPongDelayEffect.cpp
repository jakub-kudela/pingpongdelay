/**
 * PingPongDelayEffect.cpp:
 *
 * Implementation of PingPongDelayEffect class deriving vst.sdk2.4
 * AudioEffectX class providing ping pong delay VST.
 *
 * @author  Jakub Kúdela
 * @version 1.0
 * @since 2012-01-21
 *
 * @see PingPongDelayEffect
 */


#include "public.sdk/source/vst2.x/audioeffectx.h"
#include "PingPongDelayUnit.h"
#include "PingPongDelayEditor.h"

#ifndef PINGPONGDELAYEFFECT_H
#include "PingPongDelayEFFECT.h"
#endif


namespace PingPongDelay
{
    // Fields holding the inicial settings for ping pong delay
    // parameters.
    /**
     * Initial buffer size of the unit.
     */
    const int PingPongDelayEffect::defaultUnitBufferSize_ = 300000;

    /**
     * Initial delay parameter of the unit.
     */
    const float PingPongDelayEffect::defaultDelayParam_ = 0.8f;

    /**
     * Initial feedoback parameter of the unit.
     */
    const float PingPongDelayEffect::defaultFeedbackParam_ = 0.25f;

    /**
     * Initial panorama parameter of the unit.
     */
    const float PingPongDelayEffect::defaultPanoramaParam_ = 0.0f;

    /**
     * Initial wet parameter of the unit.
     */
    const float PingPongDelayEffect::defaultWetParam_ = 0.25f;

    /**
     * Initial synchronization parameter of the unit.
     */
    const float PingPongDelayEffect::defaultSyncParam_ = 0.0f;


    // Fields holding the basic PING PONG DELAY VST information.
    /**
     * Number of programs of plugin.
     */
    const VstInt32 PingPongDelayEffect::numPrograms_ = 1;

    /**
     * Number of parameters of plugin.
     */
    const VstInt32 PingPongDelayEffect::numParams_ = 5;

    /**
     * Number of input channels of plugin.
     */
    const VstInt32 PingPongDelayEffect::numInputs_ = 2;

    /**
     * Number of output channels of plugin.
     */
    const VstInt32 PingPongDelayEffect::numOutputs_ = 2;

    /**
     * Stores the unique id of the plugin given by Steinberg Technologies.
     */
    const VstInt32 PingPongDelayEffect::uniqueId_ = '1c84';

    /**
     * Inicial program name of plugin.
     */
    const char* PingPongDelayEffect::defaultProgramName_ = "Default Program";

    /**
     * Name of the plugin effect.
     */
    const char* PingPongDelayEffect::effectName_ = "Ping Pong Delay";

    /**
     * String representing the product of the plugin.
     */
    const char* PingPongDelayEffect::productString_ = "Ping Pong Delay";

    /**
     * String of the vendor of the plugin.
     */
    const char* PingPongDelayEffect::vendorString_ = "Jakub Kúdela";

    /**
     * Version of the plugin.
     */
    const VstInt32 PingPongDelayEffect::vendorVersion_ = 1000;


    // Fields holding the parameter names, labels.
    /**
     * Display string of the delay parameter.
     */
    const char* PingPongDelayEffect::delayParamName_ = "Delay";

    /**
     * Display string of the feedback parameter.
     */
    const char* PingPongDelayEffect::feedbackParamName_ = "Feedback";

    /**
     * Display string of the panorama parameter.
     */
    const char* PingPongDelayEffect::panoramaParamName_ = "Panorama";

    /**
     * Display string of the wet parameter.
     */
    const char* PingPongDelayEffect::wetParamName_ = "Wet";

    /**
     * Display string of the synchronization parameter.
     */
    const char* PingPongDelayEffect::syncParamName_ = "Sync";

    /**
     * Display string of the millisecond unit.
     */
    const char* PingPongDelayEffect::msLabel_ = "ms";

    /**
     * Display string of the ratio unit.
     */
    const char* PingPongDelayEffect::ratioLabel_ = "ratio";

    /**
     * Display string of the state unit.
     */
    const char* PingPongDelayEffect::stateLabel_ = "state";

    /**
     * Display string of the off label.
     */
    const char* PingPongDelayEffect::offLabel_ = "Off";

    /**
     * Display string of the on label.
     */
    const char* PingPongDelayEffect::onLabel_ = "On";


    /**
     * Overriden AudioEffectX::AudioEffectX(audioMasterCallback audioMaster) constructor.
     * @param audioMaster an audio VST host master.
     */
    PingPongDelayEffect::PingPongDelayEffect(audioMasterCallback audioMaster) :
        AudioEffectX(audioMaster, numPrograms_, numParams_),
        unit_(defaultUnitBufferSize_,
              getTimeInfo(kVstTempoValid),
              defaultDelayParam_,
              defaultFeedbackParam_,
              defaultPanoramaParam_,
              defaultWetParam_,
              defaultSyncParam_)
    {
        // Setting stereo input and output.
        setNumInputs(numInputs_);
        setNumOutputs(numOutputs_);
        // Identifying.
        setUniqueID(uniqueId_);
        // Supporting 32bit processing.
        canProcessReplacing();

        // Setting default program name.
        vst_strncpy(programName, defaultProgramName_, kVstMaxProgNameLen);

        // Setting GUI.
        editor = new PingPongDelayEditor(this);
    }

    /**
     * Overriden AudioEffectX::~AudioEffectX() destructor.
     */
    PingPongDelayEffect::~PingPongDelayEffect()
    {
    }

    /**
     * Overriden AudioEffectX::processReplacing(float** inputs, float** outputs, VstInt32 sampleFrames) method.
     * Provides 1 second 32-bit float sample ping pong delay processing.
     * @param inputs arrays of samples for each input channel.
     * @param outputs arrays of samples for each output channel.
     * @param sampleFrames number of samples for each channel.
     */
    void PingPongDelayEffect::processReplacing(float** inputs, float** outputs, VstInt32 sampleFrames)
    {
        float* leftInputChannel  =  inputs[0];
        float* rightInputChannel  =  inputs[1];
        float* leftOutputChannel = outputs[0];
        float* rightOutputChannel = outputs[1];

        while(--sampleFrames >= 0)
        {
            // Passing the current stereo sample to the ping pong delay unit.
            StereoSample stereoSample = unit_.GetSample(StereoSample(*leftInputChannel, *rightInputChannel));
            // Rewriting output channel stereo sample with the sample returned
            // from ping pong delay unit.
            *leftOutputChannel = stereoSample.first;
            *rightOutputChannel = stereoSample.second;

            ++leftInputChannel;
            ++rightInputChannel;
            ++leftOutputChannel;
            ++rightOutputChannel;
        }
    }

    /**
     * Overriden AudioEffectX::setProgramName(char* name) method
     * Sets the program name.
     * @param name a name of program to set.
     */
    void PingPongDelayEffect::setProgramName(char* name)
    {
        vst_strncpy(programName, name, kVstMaxProgNameLen);
    }

    /**
     * Overriden AudioEffectX::getProgramName(char* name) method.
     * Gets the program name.
     * @param name where to store the name of the program.
     */
    void PingPongDelayEffect::getProgramName(char* name)
    {
        vst_strncpy(name, programName, kVstMaxProgNameLen);
    }

    /**
     * Overriden AudioEffectX::setParameter(VstInt32 index, float value) method.
     * Sets the value of a Ping Pong Delay parameter.
     * @param index an index of parameter to set.
     * @param value a value between [0, 1] to set the parameter to.
     */
    void PingPongDelayEffect::setParameter(VstInt32 index, float value)
    {
        switch (index)
        {
            case DelayParam:
                unit_.SetDelayParam(value);
                break;

            case FeedbackParam:
                unit_.SetFeedbackParam(value);
                break;

            case PanoramaParam:
                unit_.SetPanoramaParam(value);
                break;

            case WetParam:
                unit_.SetWetParam(value);
                break;

            case SyncParam:
                unit_.SetSyncParam(value);
                break;
        }

        if(editor)
        {
            ((AEffGUIEditor*)editor)->setParameter(index, value);
        }
    }

    /**
     * Overriden AudioEffectX::getParameter(VstInt32 index) method.
     * Gets the value of a Ping Pong Delay parameter.
     * @param index an index of the parameter to get value of.
     * @return value of the parameter between [0, 1].
     */
    float PingPongDelayEffect::getParameter(VstInt32 index)
    {
        float parameter = 0;
        switch (index)
        {
            case DelayParam:
                parameter = unit_.GetDelayParam();
                break;

            case FeedbackParam:
                parameter = unit_.GetFeedbackParam();
                break;

            case PanoramaParam:
                parameter = unit_.GetPanoramaParam();
                break;

            case WetParam:
                parameter = unit_.GetWetParam();
                break;

            case SyncParam:
                parameter = unit_.GetSyncParam();
                break;
        }
        return parameter;
    }

    /**
     * Overriden AudioEffectX::getParameterName(VstInt32 index, char* text) method.
     * Gets the name of a Ping Pong Delay parameter.
     * @param index an index of parameter to get name of.
     * @param text where to store name of the parameter.
     */
    void PingPongDelayEffect::getParameterName(VstInt32 index, char* text)
    {
        switch (index)
        {
            case DelayParam :
                vst_strncpy(text, delayParamName_, kVstMaxLabelLen);
                break;

            case FeedbackParam :
                vst_strncpy(text, feedbackParamName_, kVstMaxLabelLen);
                break;

            case PanoramaParam :
                vst_strncpy(text, panoramaParamName_, kVstMaxLabelLen);
                break;

            case WetParam :
                vst_strncpy(text, wetParamName_, kVstMaxLabelLen);
                break;

            case SyncParam :
                vst_strncpy(text, syncParamName_, kVstMaxLabelLen);
                break;
        }
    }

    /**
     * Overriden AudioEffectX::getParameterLabel(VstInt32 index, char* label) method.
     * Gets the label of a Ping Pong Delay parameter.
     * @param index an index of parameter to get name of.
     * @param label where to store label of the parameter.
     */
    void PingPongDelayEffect::getParameterLabel(VstInt32 index, char* label)
    {
        switch (index)
        {
            case DelayParam :
                if(unit_.IsAsync())
                {
                    vst_strncpy(label, msLabel_, kVstMaxLabelLen);
                }
                else
                {
                    vst_strncpy(label, ratioLabel_, kVstMaxLabelLen);
                }
                break;

            case FeedbackParam :
                vst_strncpy(label, ratioLabel_, kVstMaxLabelLen);
                break;

            case PanoramaParam :
                vst_strncpy(label, ratioLabel_, kVstMaxLabelLen);
                break;

            case WetParam :
                vst_strncpy(label, ratioLabel_, kVstMaxLabelLen);
                break;

            case SyncParam :
                vst_strncpy(label, stateLabel_, kVstMaxLabelLen);
                break;
        }
    }

    /**
     * Overriden AudioEffectX::getParameterDisplay(VstInt32 index, char* text) method.
     * Gets the display of a Ping Pong Delay parameter.
     * @param index an index of parameter to get name of.
     * @param text where to store display of the parameter.
     */
    void PingPongDelayEffect::getParameterDisplay (VstInt32 index, char* text)
    {
        switch (index)
        {
            case DelayParam :
                if(unit_.IsAsync())
                {
                    int2string(unit_.GetAsyncDelayMs(), text, kVstMaxParamStrLen);
                }
                else
                {
                    vst_strncpy(text, unit_.GetSyncDelayRatio(), kVstMaxParamStrLen);
                }
                break;

            case FeedbackParam :
                float2string(unit_.GetFeedback(), text, kVstMaxParamStrLen);
                break;

            case PanoramaParam :
                float2string(unit_.GetPanoramaParam(), text, kVstMaxParamStrLen);
                break;

            case WetParam :
                float2string(unit_.GetWetParam(), text, kVstMaxParamStrLen);
                break;

            case SyncParam :
                if(unit_.IsAsync())
                {
                    vst_strncpy(text, offLabel_, kVstMaxParamStrLen);
                }
                else
                {
                    vst_strncpy(text, onLabel_, kVstMaxParamStrLen);
                }
                break;
        }
    }

    /**
     * Overriden AudioEffectX::getEffectName(char* name) method.
     * Gets the name of the Ping Pong Delay.
     * @param name where to store the name of the effect.
     * @return true, beacause the method is supported.
     */
    bool PingPongDelayEffect::getEffectName(char* name)
    {
        vst_strncpy(name, effectName_, kVstMaxEffectNameLen);
        return true;
    }

    /**
     * Overriden AudioEffectX::getVendorString(char* text) method.
     * Gets the string of vendor of the Ping Pong Delay.
     * @param text where to store the vendor string of the effect.
     * @return true, beacause the method is supported.
     */
    bool PingPongDelayEffect::getVendorString (char* text)
    {
        vst_strncpy(text, vendorString_, kVstMaxVendorStrLen);
        return true;
    }

    /**
     * Overriden AudioEffectX::getProductString(char* text) method.
     * Gets the string of product of the Ping Pong Delay.
     * @param text where to store the product string of the effect.
     * @return true, beacause the method is supported.
     */
    bool PingPongDelayEffect::getProductString(char* text)
    {
        vst_strncpy(text, productString_, kVstMaxProductStrLen);
        return true;
    }

    /**
     * Overriden AudioEffectX::getVendorVersion() method.
     * Gets the version of the Ping Pong Delay.
     * f.e.: 1000 stands for 1.0.0.0.
     * @return version.
     */
    VstInt32 PingPongDelayEffect::getVendorVersion()
    {
        return vendorVersion_;
    }

    /**
     * Overriden AudioEffectX::getPlugCategory() method.
     * Gets the category of the Ping Pong Delay.
     * @return kPlugCategRoomFx, Ping Pong Delay cateory.
     */
    VstPlugCategory PingPongDelayEffect::getPlugCategory()
    {
        return kPlugCategRoomFx;
    }
}
