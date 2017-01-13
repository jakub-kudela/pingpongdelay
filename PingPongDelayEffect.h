/**
 * PingPongDelayEffect.h:
 *
 * Declaration of PingPongDelayParameter enum for ping pong delay
 * parameters reference.
 *
 * Declaration of PingPongDelayEffect class deriving vst.sdk2.4
 * AudioEffectX class providing ping pong delay VST.
 *
 * @author  Jakub Kúdela
 * @version 1.0
 * @since 2012-01-21
 *
 * @see PingPongDelayEffect
 * @see PingPongDelayParameter
 */


#include "public.sdk/source/vst2.x/audioeffectx.h"
#include "PingPongDelayUnit.h"

#ifndef PINGPONGDELAYEFFECT_H
#define PINGPONGDELAYEFFECT_H


namespace PingPongDelay
{
    /**
     * An enum for ping pong delay parameters reference.
     */
    enum PingPongDelayParameter
    {
        DelayParam,
        FeedbackParam,
        PanoramaParam,
        WetParam,
        SyncParam,
    };

    /**
     * Class deriving vst.sdk2.4 AudioEffectX class providing
     * ping pong delay VST.
     */
    class PingPongDelayEffect : public AudioEffectX
    {
    public:
        /**
         * Overriden AudioEffectX::AudioEffectX(audioMasterCallback audioMaster) constructor.
         * @param audioMaster an audio VST host master.
         */
        PingPongDelayEffect(audioMasterCallback audioMaster);

        /**
         * Overriden AudioEffectX::~AudioEffectX() destructor.
         */
        ~PingPongDelayEffect();

        /**
         * Overriden AudioEffectX::processReplacing(float** inputs, float** outputs, VstInt32 sampleFrames) method.
         * Provides 1 second 32-bit float sample ping pong delay processing.
         * @param inputs arrays of samples for each input channel.
         * @param outputs arrays of samples for each output channel.
         * @param sampleFrames number of samples for each channel.
         */
        void processReplacing(float** inputs, float** outputs, VstInt32 sampleFrames);

        /**
         * Overriden AudioEffectX::setProgramName(char* name) method
         * Sets the program name.
         * @param name a name of program to set.
         */
        void setProgramName(char* name);

        /**
         * Overriden AudioEffectX::getProgramName(char* name) method.
         * Gets the program name.
         * @param name where to store the name of the program.
         */
        void getProgramName(char* name);

        /**
         * Overriden AudioEffectX::setParameter(VstInt32 index, float value) method.
         * Sets the value of a Ping Pong Delay parameter.
         * @param index an index of parameter to set.
         * @param value a value between [0, 1] to set the parameter to.
         */
        void setParameter(VstInt32 index, float value);

        /**
         * Overriden AudioEffectX::getParameter(VstInt32 index) method.
         * Gets the value of a Ping Pong Delay parameter.
         * @param index an index of the parameter to get value of.
         * @return value of the parameter between [0, 1].
         */
        float getParameter(VstInt32 index);

        /**
         * Overriden AudioEffectX::getParameterName(VstInt32 index, char* text) method.
         * Gets the name of a Ping Pong Delay parameter.
         * @param index an index of parameter to get name of.
         * @param text where to store name of the parameter.
         */
        void getParameterName(VstInt32 index, char* text);

        /**
         * Overriden AudioEffectX::getParameterLabel(VstInt32 index, char* label) method.
         * Gets the label of a Ping Pong Delay parameter.
         * @param index an index of parameter to get name of.
         * @param label where to store label of the parameter.
         */
        void getParameterLabel(VstInt32 index, char* label);

        /**
         * Overriden AudioEffectX::getParameterDisplay(VstInt32 index, char* text) method.
         * Gets the display of a Ping Pong Delay parameter.
         * @param index an index of parameter to get name of.
         * @param text where to store display of the parameter.
         */
        void getParameterDisplay(VstInt32 index, char* text);

        /**
         * Overriden AudioEffectX::getEffectName(char* name) method.
         * Gets the name of the Ping Pong Delay.
         * @param name where to store the name of the effect.
         * @return true, beacause the method is supported.
         */
        bool getEffectName(char* name);

        /**
         * Overriden AudioEffectX::getVendorString(char* text) method.
         * Gets the string of vendor of the Ping Pong Delay.
         * @param text where to store the vendor string of the effect.
         * @return true, beacause the method is supported.
         */
        bool getVendorString(char* text);

        /**
         * Overriden AudioEffectX::getProductString(char* text) method.
         * Gets the string of product of the Ping Pong Delay.
         * @param text where to store the product string of the effect.
         * @return true, beacause the method is supported.
         */
        bool getProductString(char* text);

        /**
         * Overriden AudioEffectX::getVendorVersion() method.
         * Gets the version of the Ping Pong Delay.
         * f.e.: 1000 stands for 1.0.0.0.
         * @return version.
         */
        VstInt32 getVendorVersion();

        /**
         * Overriden AudioEffectX::getPlugCategory() method.
         * Gets the category of the Ping Pong Delay.
         * @return kPlugCategRoomFx, Ping Pong Delay cateory.
         */
        VstPlugCategory getPlugCategory();

    private:
        /**
         * String for the current program name.
         */
        char programName[kVstMaxProgNameLen];

        /**
         * Ping pong delay processing unit.
         */
        PingPongDelayUnit unit_;


        // Fields holding the inicial settings for ping pong delay
        // parameters.
        /**
         * Initial buffer size of the unit.
         */
        static const int defaultUnitBufferSize_;

        /**
         * Initial delay parameter of the unit.
         */
        static const float defaultDelayParam_;

        /**
         * Initial feedoback parameter of the unit.
         */
        static const float defaultFeedbackParam_;

        /**
         * Initial panorama parameter of the unit.
         */
        static const float defaultPanoramaParam_;

        /**
         * Initial wet parameter of the unit.
         */
        static const float defaultWetParam_;

        /**
         * Initial synchronization parameter of the unit.
         */
        static const float defaultSyncParam_;


        // Fields holding the basic PING PONG DELAY VST information.
        /**
         * Number of programs of plugin.
         */
        static const VstInt32 numPrograms_;

        /**
         * Number of parameters of plugin.
         */
        static const VstInt32 numParams_;

        /**
         * Number of input channels of plugin.
         */
        static const VstInt32 numInputs_;

        /**
         * Number of output channels of plugin.
         */
        static const VstInt32 numOutputs_;

        /**
         * Stores the unique id of the plugin given by Steinberg Technologies.
         */
        static const VstInt32 uniqueId_;

        /**
         * Inicial program name of plugin.
         */
        static const char* defaultProgramName_;

        /**
         * Name of the plugin effect.
         */
        static const char* effectName_;

        /**
         * String representing the product of the plugin.
         */
        static const char* productString_;

        /**
         * String of the vendor of the plugin.
         */
        static const char* vendorString_;

        /**
         * Version of the plugin.
         */
        static const VstInt32 vendorVersion_;


        // Fields holding the parameter names, labels and displays.
        /**
         * Display string of the delay parameter.
         */
        static const char* delayParamName_;

        /**
         * Display string of the feedback parameter.
         */
        static const char* feedbackParamName_;

        /**
         * Display string of the panorama parameter.
         */
        static const char* panoramaParamName_;

        /**
         * Display string of the wet parameter.
         */
        static const char* wetParamName_;

        /**
         * Display string of the synchronization parameter.
         */
        static const char* syncParamName_;

        /**
         * Display string of the millisecond unit.
         */
        static const char* msLabel_;

        /**
         * Display string of the ratio unit.
         */
        static const char* ratioLabel_;

        /**
         * Display string of the state unit.
         */
        static const char* stateLabel_;

        /**
         * Display string of the off label.
         */
        static const char* offLabel_;

        /**
         * Display string of the on label.
         */
        static const char* onLabel_;
    };
}


#endif
