#ifndef  _Chess_H
#define  _Chess_H

#include <BaseGame.h>
#include "OptionDialog.h"
#include "ChessScene.h"

class Chess : public BaseGame
{
public:
    Chess();
    ~Chess();
        
protected:
    virtual void initGUI();
    void initMainMenu();
    void initOptionsMenu();
    void initCreditsMenu();
    void initPromotionMenu();
    void initKeysMenu();
    void initGameOverMenu();
    
    virtual void renderGUI();
    
    virtual void sendKeyEvent(unsigned char key, bool down);
    virtual void sendVirtualKeyEvent(GK_VirtualKey virtualKey, bool down, const ModifierKeysStates& modifierStates);
    virtual void sendMouseEvent(MouseEvent event, const ModifierKeysStates &modifierStates);
    virtual void onSizeChanged();
        
    void onNewGame(ButtonUI* newGameButton);
    void onContinueGame(ButtonUI* continuButton);
    void onOptions(ButtonUI* optionsButton);
    void onCredits(ButtonUI* creditsButton);
    void onExitPressed(ButtonUI* exitButton);
    
    void onOptionMenuCancel(ButtonUI* cancelButton);
    void onOptionMenuOK(ButtonUI* okButton);
    
    void onCreditsOkClicked(ButtonUI* okbutton);
    void onMainMenuClicked(ButtonUI* mainMenuButton);
    
    DialogUI m_mainMenuDialog;
    OptionDialog m_optionDialog;
    DialogUI m_creditsDialog;
    DialogUI m_promotionGUI;
    DialogUI m_keysDialog;
    DialogUI m_gameOverDialog;
};

#endif  //_Chess_H
