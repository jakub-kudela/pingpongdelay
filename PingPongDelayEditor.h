/**
 * PingPongDelayEditor.h:
 *
 * Declaration of PingPongDelayEditor class deriving vst.sdk2.4
 * AEffGUIEditor, CControlListener classes, providing GUI for
 * PingPongDelayEffect functionality.
 *
 * @author  Jakub Kúdela
 * @version 1.0
 * @since 2012-01-21
 *
 * @see PingPongDelayEditor
 * @see PingPongDelayEffect
 */


#include "vstgui.sf/vstgui/vstgui.h"

#ifndef GUIEDITOR_H
#define GUIEDITOR_H


namespace PingPongDelay
{
    /**
     * Class deriving vst.sdk2.4 AEffGUIEditor, CControlListener classes,
     * providing GUI for PingPongDelayEffect functionality.
     */
    class PingPongDelayEditor : public AEffGUIEditor, public CControlListener
    {
    public:
        /**
         * Overriden AEffGUIEditor::AEffGUIEditor(AudioEffect* effect) constructor.
         * @param effect a plug-in object.
         */
        PingPongDelayEditor(AudioEffect* effect);

        /**
         * Overriden AEffGUIEditor::~AEffGUIEditor() destructor.
         */
        ~PingPongDelayEditor();

        /**
         * Overriden AEffGUIEditor::open(void *ptr) method.
         * Callen when VST host is about to open a window for the editor.
         * @param ptr a pointer to parent windows is platform-dependent (HWND on Windows, WindowRef on Mac)
         * @return true, beacause the method is supported.
         */
        bool open(void* ptr);

        /**
         * Overriden AEffGUIEditor::close() method.
         * Called when VST host is about to close the window for the editor.
         */
        void close();

        /**
         * Overriden AEffGUIEditor::setParameter(VstInt32 index, float value) method.
         * Called when parameter is set via editor.
         * @param index an index of a parameter.
         * @param value a value from [0, 1] of parameter to set.
         */
        void setParameter(VstInt32 index, float value);

        /**
         * Overriden CControlListener::valueChanged(CDrawContext* context, CControl* control) method.
         * Called when anything is changed elsewhere than in editor.
         * @param context a draw context of change.
         * @param control a control of change.
         */
        void valueChanged(CDrawContext* context, CControl* control);

    private:
        /**
         * GUI Background bitmap.
         */
        CBitmap* guiBackground_ ;

        // GUI faders.
        /**
         * GUI delay faders.
         */
        CHorizontalSlider* delayFader_;

        /**
         * GUI feedback faders.
         */
        CHorizontalSlider* feedbackFader_;

        /**
         * GUI panorama faders.
         */
        CHorizontalSlider* panoramaFader_;

        /**
         * GUI wet faders.
         */
        CHorizontalSlider* wetFader_;


        /**
         * GUI synchronization button.
         */
        COnOffButton* syncButton_;


        // Fields holding the PingPongDelayEditor settings.
        // Resource bitmap ids.
        /**
         * Id of background bitmap resource.
         */
        static const short backgroundBitmapId_;

        /**
         * Id of fader background bitmap resource.
         */
        static const short faderBackgroundBitmapId_;

        /**
         * Id of fader bitmap resource.
         */
        static const short faderBitmapId_;

        /**
         * Id of synchronization button bitmap resource.
         */
        static const short syncButtonBitmapId_;


        // Fields holding fader positions.
        /**
         * Faders horizontal coor GUI position.
         */
        static const short faderX_;

        /**
         * Delay fader vertical coor GUI position.
         */
        static const short delayFaderY_;

        /**
         * Feedback fader vertical coor GUI position.
         */
        static const short feedbackFaderY_;

        /**
         * Panorama fader vertical coor GUI position.
         */
        static const short panoramaFaderY_;

        /**
         * Wet fader vertical coor GUI position.
         */
        static const short wetFaderY_;


        // Sync button position.
        /**
         * Sync button horizontal coor GUI position.
         */
        static const short syncButtonX_;

        /**
         * Sync button vertical coor GUI position.
         */
        static const short syncButtonY_;
    };
}


#endif
