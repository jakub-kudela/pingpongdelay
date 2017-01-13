/**
 * PingPongDelayUnit.h:
 *
 * Declaration of PingPongDelayUnit class providing ping pong
 * delay processing of streo sample stream.
 *
 * @author  Jakub Kúdela
 * @version 1.0
 * @since 2012-01-21
 *
 * @see PingPongDelayUnit
 */


#include <utility>
#include "public.sdk/source/vst2.x/audioeffectx.h"

#ifndef PINGPONGDELAYUNIT_H
#define PINGPONGDELAYUNIT_H


namespace PingPongDelay
{
    using std::pair;


    /**
     * Stereo sample is used by PingPongDelayUnit as input, output
     * type of sample processing.
     * First field of pair represents the left channel and
     * second represents the right channel of the sample.
     *
     * @see PingPongDelayUnit
     */
    typedef pair<float, float> StereoSample;


    /**
     * Unit providing ping pong delay processing of streo sample
     * stream.
     *
     * ALERT: Whole class requires correct usage as written
     * in documentation. It does not make any argument checks
     * nor it throws any own error,
     */
    class PingPongDelayUnit
    {
    public:
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
        PingPongDelayUnit(int bufferSize, VstTimeInfo* timeInfo, float delayParam, float feedbackParam,
                          float panoramaParam, float wetParam, float syncParam);

        /**
         * A destructor.
         */
        ~PingPongDelayUnit();

        /**
         * PingPongDelayUnit sample stream processing method.
         * @param input a next sample in stream of stereo samples to be processed by unit.
         * @return effected input stereo sample.
         */
        StereoSample GetSample(StereoSample input);

        /**
         * Gets the delay parameter setting of unit.
         * @return delay parameter between [0, 1].
         */
        float GetDelayParam();

        /**
         * Sets the delay parameter setting of unit.
         * @param delayParam a new delay parameter of the unit. Must be a
         *      value from [0, 1]. In case the unit is asynchronous with its time info
         * sets the delay corresponding to the value from [min delay time, max delay time].
         * Otherwise it sets the delay to the corresponding synchronized ratio of the tempo.
         */
         void SetDelayParam(float delayParam);

        /**
         * Gets the currently set asynchronous delay time of unit in milliseconds.
         * Method does not consider whether the unit is in synchronized mode or not.
         * @return asynchronous delay time in milliseconds.
         */
        int GetAsyncDelayMs();

        /**
         * Gets the string of currently set synchronization ratio. Method does not
         * consider whether the unit is in synchronized mode or not.
         * @return string of the ratio.
         */
        const char* GetSyncDelayRatio();

        /**
         * Gets the feedback parameter setting of unit.
         * @return feedback parameter between [0, 1]. Feedback is ratio of signal to the next
         *      delay of that signal. Returns zero if the feedback is
         *      equal to min feedback setting. Returns one it the feedback is equal to
         *      the max feedback setting. Any other settings acts evenly
         *      corresponding to the value.
         */
        float GetFeedbackParam();

        /**
         * Sets the feedback parameter setting of unit.
         * @param feedbackParam a new feedback parameter of the unit. Must be a
         *      value from [0, 1]. Feedback is ratio of signal to the next
         *      delay of that signal. Setting it to zero makes the feedback equal to
         *      min feedback setting. Setting it to one makes the feedback equal to
         *      the max feedback setting. Any other settings in between these act evenly
         *      corresponding to the value.
         */
        void SetFeedbackParam(float feedbackParam);

        /**
         * Gets the feedback setting of unit.
         * @return feedback a ratio of signal to the next
         *      delay of that signal.
         */
        float GetFeedback();

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
        float GetPanoramaParam();

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
        void SetPanoramaParam(float panoramaParam);

        /**
         * Gets the wet parameter setting of unit.
         * @return wet parameter between [0, 1].
         *      Returned value is a value of ratio of wet/dry signal of
         *      output stereo samples streaming through the unit.
         */
        float GetWetParam();

        /**
         * Sets the wet parameter setting of unit.
         * @param wetParam a new wet parameter of the unit. Must be a
         *      value from [0, 1]. Wet will be a new ratio of
         *      wet/dry signal of output stereo samples streaming
         *      through the unit.
         */
        void SetWetParam(float wetParam);

        /**
         * Gets the synchronization parameter setting of unit.
         * @return synchronization parameter between [0, 1].
         *      Returns zero if the unit is asynchronous and any
         *      other value means the unit is synchronized to its time
         *      info tempo.
         */
        float GetSyncParam();

        /**
         * Sets the synchronization parameter setting of unit.
         * @param syncParam a new synchronization parameter of the unit. Must be a
         *      value from [0, 1]. Setting it to zero will make the unit
         *      asynchronous and any other value will make it synchronize
         *      to ist time info tempo.
         */
        void SetSyncParam(float syncParam);

        /**
         * Tells whether the unit is in asynchronous mode with its time info tempo.
         * @return true if the unit is asynchronous, false otherwise.
        */
        bool IsAsync();

    private:
        /**
         * Increments the inner buffer cursors.
         */
        void IncrementBufferCursor();

        /**
         * Proper math modulo arithmetic function for the cursor not
         * to exceed bounds of the buffer arrays while calculating
         * delayed index.
         * @param cursorIndex a buffer index, does not matter if exceeded
         *   outside from buffer arrays.
         * @return (cursorIndex) mod (buffer size).
         */
        int BufferModulo(int cursorIndex);

        /**
         * Calculates evenly corresponding integer value from interval
         * [min, max] to value from interval [0, 1].
         * @param param a parameter value from interval [0, 1].
         * @param min a lower bound of closed output interval.
         * @param max an upper bound of closed output interval.
         * @return corresponding int value from [min, max].
         */
        int Corresponding(float param, int min, int max);

        /**
         * Calculates evenly corresponding float value from interval
         * [min, max] to value from interval [0, 1].
         * @param param a parameter value from interval [0, 1].
         * @param min a lower bound of closed output interval.
         * @param max an upper bound of closed output interval.
         * @return corresponding float value from [min, max].
         */
        float Corresponding(float param, float min, float max);


        /**
         * Size of the stereo buffer.
         *
         * @see leftBuffer_
         * @see rightBuffer_
         */
        int bufferSize_;

        /**
         * Inner time info containing valid tempo information.
         */
        VstTimeInfo* timeInfo_;


        // Fields for parameters of unit settings.
        /**
         * Stores the value of delay parameter.
         */
        float delayParam_;

        /**
         * Stores the value of feedback parameter.
         */
        float feedbackParam_;

        /**
         * Stores the value of panorama parameter.
         */
        float panoramaParam_;

        /**
         * Stores the value of wet parameter.
         */
        float wetParam_;

        /**
         * Stores the value of synchronization parameter.
         */
        float syncParam_;


        // Auxiliary fields for unit settings.
        /**
         * Field containing corresponding delay time in milliseconds set by
         * delay parameter value while unit is usynchronized with its time info
         * inner tempo.
         *
         * @see timeInfo_
         */
        int asyncDelayMs_;

        /**
         * This index points to the syncRatios_ index of which ratio is
         * currently set by delay parameter value while unit is synchronized
         * with its time info inner tempo.
         *
         * @see syncRatios_
         * @see timeInfo_
         */
        int syncDelayRatioIndex_;

        /**
         * Field representing the precalculated correspondening feedback value
         * to the interval [minFeedback_, maxFeedback_]set by feedbackParam_.
         *
         * @see minFeedback_
         * @see maxFeedback_
         * @see feedbackParam_
         */
        float feedback_;

        /**
         * This field together with panoramaParam_ are used to provide ping
         * pong delaying. They represent the ratio of delaying signal in one
         * channel in comparison to the other. They are always complementary
         * so their sum must be equal to 1.
         *
         * @see panoramaParam_
         */
        float panoramaParamC_;

        /**
         * primaryPanningQuotient_ & secondaryPanningQuotient_ are used to provide
         * stereo delaying. They represent the ratio of respectively left and
         * right channel input sample to be delayed in left channel and the
         * ratio of respectively right and left channel input sample to be
         * delayed in right channel. They are always complementary. Their
         * sum must be equal to 1.
         *
         * @see secondaryPanningQuotient_
         */
        float primaryPanningQuotient_;

        /**
         * primaryPanningQuotient_ & secondaryPanningQuotient_ are used to provide
         * stereo delaying. They represent the ratio of respectively left and
         * right channel input sample to be delayed in left channel and the
         * ratio of respectively right and left channel input sample to be
         * delayed in right channel. They are always complementary. Their
         * sum must be equal to 1.
         *
         * @see primaryPanningQuotient_
         */
        float secondaryPanningQuotient_;

        /**
         * This field together with wetParam_ are complementary so that
         * their sum must be equal to 1. In This ratio is also called a "dry"
         * ratio in audio effect terminology. Wet represents the ratio of
         * effected while dry represents the ratio of original signal in output.
         *
         * @see wetParam_
         */
        float wetParamC_;

        /**
         * This field stores the precalculated value representing weather the
         * unit is synchronized ("true") with its time info or not ("false").
         */
        bool isAsync_;


        /**
         * Buffer cursor pointing to the index where should be current sample
         * being played written.
         */
        int bufferCursor_;

        /**
         * An array of samples representing left channel circular buffer.
         * Purpose of this buffers is to track down the dry signal combined with
         * feedbacked delayed signal on left channel.
         */
        float* leftBuffer_;

        /**
         * An array of samples representing right channel circular buffer.
         * Purpose of this buffers is to track down the dry signal combined with
         * feedbacked delayed signal on right channel.
         */
        float* rightBuffer_;


        // Fields representing the possible settings of
        // delaying time of unit while it is asynchronous.
        /**
         * Lower bound of possible asynchronous delay settings in ms.
         */
        static const int minAsyncDelayMs_;

        /**
         * Upper bound of possible asynchronous delay settings in ms.
         */
        static const int maxAsyncDelayMs_;


        // Fields representing the possible settings of
        // delaying time of unit while it is synchronized.
        /**
         * Stores the number of possible synchronized delay ratio settings.
         * It is the size of syncDelayRatios_ array.
         *
         * @see syncDelayRatios_
         */
        static const int syncDelayRatioCount_;

        /**
         * Stores the possible synchronized delay ratios.
         */
        static const float syncDelayRatios_[];

        /**
         * Stores the strings of possible synchronized delay ratios,
         * from syncDelayRatios_.
         *
         * @see syncDelayRatios_.
         */
        static const char* syncDelayRatioStrings_[];


        // Field representing the bounds of feedback ratio.
        // These constrictions are made due to the protection from
        // output signal clipping.
        /**
         * Lower bound of possible feedback ratio settings.
         */
        static const float minFeedback_;
        /**
         * Upper bound of possible feedback ratio settings.
         */
        static const float maxFeedback_;


        // Fields representing ratios of time convertions.
        /**
         * Stores how many seconds are in minute.
         */
        static const int sInMin_;

        /**
         * Stores how many milliseconds are in second.
         */
        static const int msInS_;
    };
}


#endif
