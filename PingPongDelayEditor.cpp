/**
 * PingPongDelayEditor.cpp:
 *
 * Implementation of PingPongDelayEditor class deriving vst.sdk2.4
 * AEffGUIEditor, CControlListener classes, providing GUI for PingPongDelayEffect functionality.
 *
 * @author  Jakub Kúdela
 * @version 1.0
 * @since 2012-01-21
 *
 * @see PingPongDelayEditor
 * @see PingPongDelayEffect
 */


#include <stdio.h>
#include "PingPongDelayEffect.h"

#ifndef PINGPONGDELAYEDITOR_H
#include "PingPongDelayEditor.h"
#endif


namespace PingPongDelay
{
    // Fields holding the PingPongDelayEditor settings.
    // Bitmap ids.
    /**
     * Id of background bitmap resource.
     */
    const short PingPongDelayEditor::backgroundBitmapId_ = 1;

    /**
     * Id of fader background bitmap resource.
     */
    const short PingPongDelayEditor::faderBackgroundBitmapId_ = 2;

    /**
     * Id of fader bitmap resource.
     */
    const short PingPongDelayEditor::faderBitmapId_ = 3;

    /**
     * Id of synchronization button bitmap resource.
     */
    const short PingPongDelayEditor::syncButtonBitmapId_ = 4;


    // Fields holding fader positions.
    /**
     * Faders horizontal coor GUI position.
     */
    const short PingPongDelayEditor::faderX_ = 42;

    /**
     * Delay fader vertical coor GUI position.
     */
    const short PingPongDelayEditor::delayFaderY_ = 83;

    /**
     * Feedback fader vertical coor GUI position.
     */
    const short PingPongDelayEditor::feedbackFaderY_ = 153;

    /**
     * Panorama fader vertical coor GUI position.
     */
    const short PingPongDelayEditor::panoramaFaderY_ = 209;

    /**
     * Wet fader vertical coor GUI position.
     */
    const short PingPongDelayEditor::wetFaderY_ = 264;

    // Sync button position.
    const short PingPongDelayEditor::syncButtonX_ = 131;
    const short PingPongDelayEditor::syncButtonY_ = 316;


    /**
     * Overriden AEffGUIEditor::AEffGUIEditor(AudioEffect* effect) constructor.
     * @param effect a plug-in object.
     */
    PingPongDelayEditor::PingPongDelayEditor(AudioEffect *effect) :
        AEffGUIEditor (effect),
        delayFader_(NULL),
        feedbackFader_(NULL),
        panoramaFader_(NULL),
        wetFader_(NULL),
        syncButton_(NULL)
    {
        // Loading GUI background bitmap.
        guiBackground_  = new CBitmap(backgroundBitmapId_);

        // Inicializing the GUI size.
        rect.left   = 0;
        rect.top    = 0;
        rect.right  = guiBackground_->getWidth();
        rect.bottom = guiBackground_->getHeight();
    }

    /**
     * Overriden AEffGUIEditor::~AEffGUIEditor() destructor.
     */
    PingPongDelayEditor::~PingPongDelayEditor()
    {
        // Forgetting background bitmap.
        if(guiBackground_)
        {
            guiBackground_->forget();
        }
        guiBackground_ = NULL;
    }

    /**
     * Overriden AEffGUIEditor::open(void *ptr) method.
     * Callen when VST host is about to open a window for the editor.
     * @param ptr a pointer to parent windows is platform-dependent (HWND on Windows, WindowRef on Mac)
     * @return true, beacause the method is supported.
     */
    bool PingPongDelayEditor::open(void *ptr)
    {
        // While opening it is necessary to call original open method!!!
        AEffGUIEditor::open(ptr);

        // Creating new CFrame for GUI.
        CBitmap* faderBackgroundBitmap = new CBitmap(faderBackgroundBitmapId_);
        CRect guiSizeRect(0, 0, guiBackground_->getWidth(), guiBackground_->getHeight());
        CFrame* guiFrame = new CFrame(guiSizeRect, ptr, this);
        guiFrame->setBackground(guiBackground_);

        // Creating faders.
        CBitmap* faderBitmap = new CBitmap(faderBitmapId_);
        int minXFaderPos = faderX_;
        int maxXFaderPos = faderX_ + faderBackgroundBitmap->getWidth() - faderBitmap->getWidth();
        CPoint offset(0, 0);

        // Creating Delay fader.
        CRect delayFaderSizeRect(faderX_, delayFaderY_, faderX_ + faderBackgroundBitmap->getWidth(), delayFaderY_ + faderBackgroundBitmap->getHeight());
        delayFader_ = new CHorizontalSlider(delayFaderSizeRect, this, DelayParam, minXFaderPos, maxXFaderPos, faderBitmap, faderBackgroundBitmap, offset, kLeft);
        delayFader_->setValue(effect->getParameter(DelayParam));
        guiFrame->addView(delayFader_);

        // Creating Feedback fader.
        CRect feedbackFaderSizeRect(faderX_, feedbackFaderY_, faderX_ + faderBackgroundBitmap->getWidth(), feedbackFaderY_ + faderBackgroundBitmap->getHeight());
        feedbackFader_ = new CHorizontalSlider(feedbackFaderSizeRect, this, FeedbackParam, minXFaderPos, maxXFaderPos, faderBitmap, faderBackgroundBitmap, offset, kLeft);
        feedbackFader_->setValue(effect->getParameter(FeedbackParam));
        guiFrame->addView(feedbackFader_);

        // Creating Panorama fader.
        CRect panoramaFaderSizeRect(faderX_, panoramaFaderY_, faderX_ + faderBackgroundBitmap->getWidth(), panoramaFaderY_ + faderBackgroundBitmap->getHeight());
        panoramaFader_ = new CHorizontalSlider(panoramaFaderSizeRect, this, PanoramaParam, minXFaderPos, maxXFaderPos, faderBitmap, faderBackgroundBitmap, offset, kLeft);
        panoramaFader_->setValue(effect->getParameter(PanoramaParam));
        guiFrame->addView(panoramaFader_);

        // Creating Wet fader.
        CRect wetFaderSizeRect(faderX_, wetFaderY_, faderX_ + faderBackgroundBitmap->getWidth(), wetFaderY_ + faderBackgroundBitmap->getHeight());
        wetFader_ = new CHorizontalSlider(wetFaderSizeRect, this, WetParam, minXFaderPos, maxXFaderPos, faderBitmap, faderBackgroundBitmap, offset, kLeft);
        wetFader_->setValue(effect->getParameter(WetParam));
        guiFrame->addView(wetFader_);

        // Creating Sync Button.
        CBitmap* syncButtonBitmap = new CBitmap(syncButtonBitmapId_);
        CRect syncButtonSizeRect(syncButtonX_, syncButtonY_, syncButtonX_ + syncButtonBitmap->getWidth(),
                                 syncButtonY_ + (syncButtonBitmap->getHeight() / 2));
        syncButton_ = new COnOffButton(syncButtonSizeRect, this, SyncParam, syncButtonBitmap);
        syncButton_->setValue(effect->getParameter(SyncParam));
        guiFrame->addView(syncButton_);

        // Decrementing numbers of references. We cannot delete them
        // directly as far as some CControls use these bitmaps.
        faderBackgroundBitmap->forget();
        faderBitmap->forget();
        syncButtonBitmap->forget();

        // Assigning created guiFrame as VST's GUI main frame.
        frame = guiFrame;
        return true;
    }

    /**
     * Overriden AEffGUIEditor::close() method.
     * Callend when VST host is about to close the window for the editor.
     */
    void PingPongDelayEditor::close()
    {
        // Dispozing the frame.
        delete frame;
        frame = NULL;
    }

    /**
     * Overriden AEffGUIEditor::setParameter(VstInt32 index, float value) method.
     * Called when parameter is set via editor.
     * @param index an index of a parameter.
     * @param value a value from [0, 1] of parameter to set.
     */
    void PingPongDelayEditor::setParameter (VstInt32 index, float value)
    {
        if(!frame)
        {
            return;
        }

        switch (index)
        {
            case DelayParam:
                if (delayFader_)
                    delayFader_->setValue(effect->getParameter (index));
                break;

            case FeedbackParam:
                if (feedbackFader_)
                    feedbackFader_->setValue(effect->getParameter (index));
                break;

            case PanoramaParam:
                if (panoramaFader_)
                    panoramaFader_->setValue(effect->getParameter (index));
                break;

            case WetParam:
                if (wetFader_)
                    wetFader_->setValue(effect->getParameter (index));
                break;

            case SyncParam:
                if (syncButton_)
                    syncButton_->setValue(effect->getParameter (index));
                break;
        }
    }

    /**
     * Overriden CControlListener::valueChanged(CDrawContext* context, CControl* control) method.
     * Called when anything is changed elsewhere than in editor.
     * @param context a draw context of change.
     * @param control a control of change.
     */
    void PingPongDelayEditor::valueChanged(CDrawContext* context, CControl* control)
    {
        effect->setParameterAutomated(control->getTag(), control->getValue());
        control->setDirty();
    }
}
