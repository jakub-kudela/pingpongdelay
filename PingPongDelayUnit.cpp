/**
 * PingPongDelayUnit.cpp:
 *
 * Implementation of PingPongDelayUnit class providing ping pong
 * delay processing of streo sample stream.
 *
 * @author  Jakub Kúdela
 * @version 1.0
 * @since 2012-01-21
 *
 * @see PingPongDelayUnit
 */


#include <math.h>
#include "public.sdk/source/vst2.x/audioeffectx.h"

#ifndef PINGPONGDELAY_H
#include "PingPongDelayUnit.h"
#endif


namespace PingPongDelay
{
    // Fields representing the possible settings of
    // delaying time of unit while it is asynchronous.
    /**
     * Lower bound of possible asynchronous delay settings in ms.
     */

    const int PingPongDelayUnit::minAsyncDelayMs_ = 10;
    /**
     * Upper bound of possible asynchronous delay settings in ms.
     */
    const int PingPongDelayUnit::maxAsyncDelayMs_ = 2000;

    // Fields representing the possible settings of
    // delaying time of unit while it is synchronized.
    /**
     * Stores the number of possible synchronized delay ratio settings.
     * It is the size of syncDelayRatios_ array.
     *
     * @see syncDelayRatios_
     */
    const int PingPongDelayUnit::syncDelayRatioCount_ = 7;

    /**
     * Stores the possible synchronized delay ratios.
     */
    const float PingPongDelayUnit::syncDelayRatios_[syncDelayRatioCount_] = {(1.0f / 4.0f), (1.0f / 3.0f), (1.0f / 2.0f), (2.0f / 3.0f), 1.0f, (3.0f / 2.0f), 2.0f};

    /**
     * Stores the strings of possible synchronized delay ratios,
     * from syncDelayRatios_.
     *
     * @see syncDelayRatios_.
     */
    const char* PingPongDelayUnit::syncDelayRatioStrings_[syncDelayRatioCount_] = {"1/4", "1/3", "1/2", "2/3", "1", "3/2", "2"};

    // Field representing the bounds of feedback ratio.
    // These constrictions are made due to the protection from
    // output signal clipping.
    /**
     * Lower bound of possible feedback ratio settings.
     */
    const float PingPongDelayUnit::minFeedback_ = 0.0f;

    /**
     * Upper bound of possible feedback ratio settings.
     */
    const float PingPongDelayUnit::maxFeedback_ = 0.8f;

    // Fields representing ratios of time convertions.
    /**
     * Stores how many seconds are in minute.
     */
    const int PingPongDelayUnit::sInMin_ = 60;

     /**
     * Stores how many milliseconds are in second.
     */
    const int PingPongDelayUnit::msInS_ = 1000;


    /**
     * A constructor.
     * All the parameters are between [0, 1]. The unit will be set accordingly
     * to the behavior of methods setting the parameters.
     * @param bufferSize a size of the buffers, must be greater than 3.
     *      It determinates the lower bound of time info tempo of correct
     *      synchronization functionality. The greater bufferSize means
     *      greater lower tempo range of correct synchronization.
     * @param timeInfo a timeInfo with valid tempo information. F.e. Use
     *      vstdsdk2.4 function getTimeInfo with kVstTempoValid flag.
     * @param delayParam a default delay parameter between [0, 1].
     * @param feedbackParam a default feedback parameter between [0, 1].
     * @param panoramaParam a default panorama parameter between [0, 1].
     * @param wetParam a default wet parameter between [0, 1].
     * @param syncParam a default synchronization parameter between [0, 1].
     *
     * @see SetDelayParam(float delay)
     * @see SetFeedbackParam(float feedback)
     * @see SetPanoramaParam(float panorama)
     * @see SetWetParam(float wet)
     * @see SetSyncParam(float sync)
     */
    PingPongDelayUnit::PingPongDelayUnit(int bufferSize, VstTimeInfo* timeInfo, float delayParam, float feedbackParam,
                                         float panoramaParam, float wetParam, float syncParam) :
        bufferSize_(bufferSize),
        timeInfo_(timeInfo),
        bufferCursor_(0)
    {
        // Allocating buffers.
        leftBuffer_ = new float[bufferSize];
        rightBuffer_ = new float[bufferSize];
        // Inicialize buffers by erasing.
        memset (leftBuffer_, 0, bufferSize * sizeof (float));
        memset (rightBuffer_, 0, bufferSize * sizeof (float));

        // Setting the inicial unit settings.
        SetDelayParam(delayParam);
        SetFeedbackParam(feedbackParam);
        SetPanoramaParam(panoramaParam);
        SetWetParam(wetParam);
        SetSyncParam(syncParam);
    }

    /**
     * A destructor.
     */
    PingPongDelayUnit::~PingPongDelayUnit()
    {
        //  Deleting allocated buffers by unit.
        if(leftBuffer_)
        {
            delete[] leftBuffer_;
        }
        if(rightBuffer_)
        {
            delete[] rightBuffer_;
        }
    }

    /**
     * PingPongDelayUnit sample stream processing method.
     * @param input a next sample in stream of stereo samples to be processed by unit.
     * @return effected input stereo sample.
     */
    StereoSample PingPongDelayUnit::GetSample(StereoSample input)
    {
        // Calculating a number of samples for delay.
        int delaySamples;
        if(IsAsync())
        {
            // Setting the asynchronous pre-calculated delay as the delay
            // in case that unit is asynchronous.
            float msSamples = (timeInfo_->sampleRate / msInS_);
            delaySamples = (int)(asyncDelayMs_ * msSamples);
        }
        else
        {
            // Calculating delay as number of samples in case that unit is
            // synchronized to the tempo setting.
            float beatsPerSec = timeInfo_->tempo / sInMin_;
            float samplesPerBeat = timeInfo_->sampleRate / beatsPerSec;
            delaySamples = (int)(samplesPerBeat * syncDelayRatios_[syncDelayRatioIndex_]);
        }

        // Getting semi and full delayed cursors through BufferModulo method.
        int semiDelayedCursor = BufferModulo(bufferCursor_ - delaySamples);
        int fullDelayedCursor = BufferModulo(bufferCursor_ - (delaySamples * 2));

        // Writing actual samples to buffer.
        leftBuffer_[bufferCursor_] = (input.first + leftBuffer_[fullDelayedCursor]) * feedback_;
        rightBuffer_[bufferCursor_] =  (input.second + rightBuffer_[fullDelayedCursor]) * feedback_;

        // Construction of the current output samples includes combining the dry and wet samples.
        float left = (wetParamC_ * input.first) +
            (wetParam_ * ((panoramaParamC_ * (primaryPanningQuotient_ * leftBuffer_[semiDelayedCursor] + secondaryPanningQuotient_ * rightBuffer_[semiDelayedCursor])) +
                     (panoramaParam_ * (secondaryPanningQuotient_ * leftBuffer_[fullDelayedCursor] + primaryPanningQuotient_ * rightBuffer_[fullDelayedCursor]))));
        // Constructing the actual right output sample.
        float right = (wetParamC_ * input.second) +
            (wetParam_ * ((panoramaParam_ * (primaryPanningQuotient_ * leftBuffer_[semiDelayedCursor] + secondaryPanningQuotient_ * rightBuffer_[semiDelayedCursor])) +
                     (panoramaParamC_ * (secondaryPanningQuotient_ * leftBuffer_[fullDelayedCursor] + primaryPanningQuotient_ * rightBuffer_[fullDelayedCursor]))));

        // Move buffer cursor to new position.
        IncrementBufferCursor();
        return StereoSample(left, right);
    }

    /**
     * Gets the delay parameter setting of unit.
     * @return delay parameter between [0, 1].
     */
    float PingPongDelayUnit::GetDelayParam()
    {
        return delayParam_;
    }

    /**
     * Sets the delay parameter setting of unit.
     * @param delayParam a new delay parameter of the unit. Must be a
     *      value from [0, 1]. In case the unit is asynchronous with its time info
     * sets the delay corresponding to the value from [min delay time, max delay time].
     * Otherwise it sets the delay to the corresponding synchronized ratio of the tempo.
     */
    void PingPongDelayUnit::SetDelayParam(float delayParam)
    {
        delayParam_ = delayParam;
        // Calculating the corresponding time of asynchronous delay in ms.
        asyncDelayMs_ = Corresponding(delayParam, minAsyncDelayMs_, maxAsyncDelayMs_);
        // Calculating the corresponding index of sync ratio of synchronized delay.
        syncDelayRatioIndex_ = Corresponding(delayParam, 0, syncDelayRatioCount_ - 1);
    }

    /**
     * Gets the currently set asynchronous delay time of unit in milliseconds.
     * Method does not consider whether the unit is in synchronized mode or not.
     * @return asynchronous delay time in milliseconds.
     */
    int PingPongDelayUnit::GetAsyncDelayMs()
    {
        return asyncDelayMs_;
    }

    /**
     * Gets the string of currently set synchronization ratio. Method does not
     * consider whether the unit is in synchronized mode or not.
     * @return string of the ratio.
     */
    const char* PingPongDelayUnit::GetSyncDelayRatio()
    {
        return syncDelayRatioStrings_[syncDelayRatioIndex_];
    }

    /**
     * Gets the feedback parameter setting of unit.
     * @return feedback parameter between [0, 1]. Feedback is ratio of signal to the next
     *      delay of that signal. Returns zero if the feedback is
     *      equal to min feedback setting. Returns one it the feedback is equal to
     *      the max feedback setting. Any other settings acts evenly
     *      corresponding to the value.
     */
    float PingPongDelayUnit::GetFeedbackParam()
    {
        return feedbackParam_;
    }

    /**
     * Sets the feedback parameter setting of unit.
     * @param feedbackParam a new feedback parameter of the unit. Must be a
     *      value from [0, 1]. Feedback is ratio of signal to the next
     *      delay of that signal. Setting it to zero makes the feedback equal to
     *      min feedback setting. Setting it to one makes the feedback equal to
     *      the max feedback setting. Any other settings in between these act evenly
     *      corresponding to the value.
     */
    void PingPongDelayUnit::SetFeedbackParam(float feedbackParam)
    {
        feedbackParam_ = feedbackParam;
        feedback_ = Corresponding(feedbackParam, minFeedback_, maxFeedback_);
    }

    /**
     * Gets the feedback setting of unit.
     * @return feedback a ratio of signal to the next
     *      delay of that signal.
     */
    float PingPongDelayUnit::GetFeedback()
    {
        return feedback_;
    }

    /**
     * Gets the panorama parameter setting of unit.
     * @return panorama parameter between [0, 1]. In case zero is returned unit
     *      is delaying firstly all delayed signal as mono to the left, then
     *      to the right channel. In case of one half is returned unit puts all the
     *      delayings as stereo to the center each time. In case of one is returned
     *      unit is delaying firstly all delayed signal as mono to the right, then
     *      to the left channel. Any other settings in between these act evenly
     *      corresponding to the value.
     */
    float PingPongDelayUnit::GetPanoramaParam()
    {
        return panoramaParam_;
    }

    /**
     * Sets the panorama parameter setting of unit.
     * @param panoramaParam a new panorama parameter of the unit. Must be a
     *      value from [0, 1]. Setting panorama to zero makes the unit
     *      delaying firstly all delayed signal as mono to the left, then
     *      to the right channel. Setting panorama to a one half puts all the
     *      delayings as stereo to the center each time. Setting panorama
     *      to one makes the unit delaying firstly all delayed signal as
     *      mono to the right, then to the left channel. Any other settings
     *      in between these act evenly corresponding to the value.
     */
    void PingPongDelayUnit::SetPanoramaParam(float panoramaParam)
    {
        panoramaParam_ = panoramaParam;
        // Calculating the complementary ratio to panorama.
        panoramaParamC_ = (1 - panoramaParam);
        // The reason of calculating these quotients is that when the panorama
        // of the delay unit is set to 0 or 1 (while signal delays in one channel
        // there is none delayed in the other) the delayed signal added to the
        // corresponding channel should be mono so that both original channels would
        // be heard. On the other side if the panorama is set to 0.5 (Each delay is
        // equally intensive in both channels) there should be heard the stereo
        // panning from the original sample.
        secondaryPanningQuotient_ = fabs(0.5f - primaryPanningQuotient_);
        // Calculating the complementary ratio to secondary panning quotient.
        primaryPanningQuotient_ = 1 - secondaryPanningQuotient_;
    }

    /**
     * Gets the wet parameter setting of unit.
     * @return wet parameter between [0, 1].
     *      Returned value is a value of ratio of wet/dry signal of
     *      output stereo samples streaming through the unit.
     */
    float PingPongDelayUnit::GetWetParam()
    {
        return wetParam_;
    }


    /**
     * Sets the wet parameter setting of unit.
     * @param wetParam a new wet parameter of the unit. Must be a
     *      value from [0, 1]. Wet will be a new ratio of
     *      wet/dry signal of output stereo samples streaming
     *      through the unit.
     */
    void PingPongDelayUnit::SetWetParam(float wetParam)
    {
        wetParam_ = wetParam;
        // Calculating the complement ratio to wet.
        wetParamC_ = (1 - wetParam);
    }

    /**
     * Gets the synchronization parameter setting of unit.
     * @return synchronization parameter between [0, 1].
     *      Returns zero if the unit is asychronous and any
     *      other value means the unit is synchronized to its time
     *      info tempo.
     */
    float PingPongDelayUnit::GetSyncParam()
    {
        return syncParam_;
    }

    /**
     * Sets the synchronization parameter setting of unit.
     * @param syncParam a new synchronization parameter of the unit. Must be a
     *      value from [0, 1]. Setting it to zero will make the unit
     *      asynchronous and any other value will make it synchronize
     *      to ist time info tempo.
     */
    void PingPongDelayUnit::SetSyncParam(float syncParam)
    {
        syncParam_ = syncParam;
        isAsync_ = (syncParam == 0.0f);
    }

    /**
     * Tells whether the unit is asynchronous with its time info tempo.
     * @return true if the unit is asynchronous, false otherwise.
    */
    bool PingPongDelayUnit::IsAsync()
    {
        return isAsync_;
    }

    /**
     * Increments the inner buffer cursors.
     */
    void PingPongDelayUnit::IncrementBufferCursor()
    {
        // If cursor reaches the end of the buffer it jumps to the start.
        if(bufferCursor_ != bufferSize_ - 1)
        {
            ++bufferCursor_;
        }
        else
        {
            bufferCursor_ = 0;
        }
    }

    /**
     * Proper math modulo arithmetic function for the cursor not
     * to exceed bounds of the buffer arrays while calculating
     * delayed index.
     * @param cursorIndex a buffer index, does not matter if exceeded
     *   outside from buffer arrays.
     * @return (cursorIndex) mod (buffer size).
     */
    int PingPongDelayUnit::BufferModulo(int cursorIndex)
    {
        // Proper math modulo arithmetics: http://en.wikipedia.org/wiki/Modular_arithmetic.
        return (cursorIndex % bufferSize_ + bufferSize_) % bufferSize_;
    }

    /**
     * Calculates evenly corresponding integer value from interval
     * [min, max] to value from interval [0, 1].
     * @param param a parameter value from interval [0, 1].
     * @param min a lower bound of closed output interval.
     * @param max an upper bound of closed output interval.
     * @return corresponding int value from [min, max].
     */
    int PingPongDelayUnit::Corresponding(float param, int min, int max)
    {
        if (param == 1.0f)
        {
            return max;
        }

        // For each int value within the interval there is equally big
        // corresponding interval from [0, 1]. Explicit conversion from
        // float to int gives the same outcome as floor from float.
        int range = max - min + 1;
        return (int)((param * range) + min);
    }

    /**
     * Calculates evenly corresponding float value from interval
     * [min, max] to value from interval [0, 1].
     * @param param a parameter value from interval [0, 1].
     * @param min a lower bound of closed output interval.
     * @param max an upper bound of closed output interval.
     * @return corresponding float value from [min, max].
     */
    float PingPongDelayUnit::Corresponding(float param, float min, float max)
    {
        float range = max - min;
        return ((range * param) + min);
    }
}
