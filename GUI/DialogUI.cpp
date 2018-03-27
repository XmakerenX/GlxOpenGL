#include "DialogUI.h"
#include <iostream>
#include <limits>

ControlUI* DialogUI::s_pControlFocus = nullptr;

//-----------------------------------------------------------------------------
// Name : DialogUI (constructor)
//-----------------------------------------------------------------------------
DialogUI::DialogUI(void)
{
    m_nCaptionHeight = 18;

    m_bVisible = true;
    m_bCaption = true;
    m_bMinimized = false;
    m_bDrag = false;

    m_x = 0;
    m_y = 0;

    m_width = 0;
    m_height = 0;
    m_texWidth = 0;
    m_texHeight = 0;

    m_texturePath[0] = '\0';
    m_captionText[0] = '\0';

    m_pMouseOverControl = nullptr;

    s_pControlFocus = nullptr;
}


//-----------------------------------------------------------------------------
// Name : DialogUI (destructor)
//-----------------------------------------------------------------------------
DialogUI::~DialogUI(void)
{
    for (GLuint i = 0; i < m_Controls.size(); i++)
    {
        if (m_Controls[i])
        {
            delete m_Controls[i];
            m_Controls[i] = nullptr; // just to be safe
        }
    }

    m_Controls.clear();
}

//-----------------------------------------------------------------------------
// Name : init ()
// Desc : initialize the dialog size , texture and caption
//-----------------------------------------------------------------------------
bool DialogUI::init(GLuint width, GLuint height, int nCaptionHeight, std::string captionText, std::string newTexturePath, glm::vec4 dialogColor, AssetManager& assetManger)
{
    GLuint textureName;

    setSize(width,height);
    m_nCaptionHeight = nCaptionHeight;
    m_captionText = captionText;

    m_rcBoundingBox = Rect(m_x, m_y, m_x + m_width, m_y + m_height);
    m_rcCaptionBox = Rect(m_x, m_y , m_x + m_width, m_y + m_nCaptionHeight);

    // sets dialog background texture
    // checks first that there is a texutre to load
    if (newTexturePath.empty())
    {
        m_texWidth = 0;
        m_texHeight = 0;
    }
    else
    {
        // load the texture for the dialog background
        textureName = assetManger.getTexture(newTexturePath);
        // get the texture width and height
        glGetTextureLevelParameteriv(textureName, 0, GL_TEXTURE_WIDTH, &m_texWidth);
        glGetTextureLevelParameteriv(textureName, 0, GL_TEXTURE_HEIGHT, &m_texHeight);

        m_texturePath = newTexturePath;
    }

    m_dialogColor = dialogColor;

    //initWoodControlElements(assetManger);

    return true;
}

//-----------------------------------------------------------------------------
// Name : initDefControlElements ()
// Desc : initialize defualt textures and fonts used by GUI
//-----------------------------------------------------------------------------
bool DialogUI::initDefControlElements(AssetManager &assetManger)
{
    std::vector<ELEMENT_GFX>  elementGFXvec;
    std::vector<ELEMENT_FONT> elementFontVec;
    CONTROL_GFX	controlGFX;

    GLuint textureIndex, fontIndex;

    // create the controls font
//    if (!assetManger.getFont("times new roman",12, 12, FW_BOLD, FALSE, fontIndex))
//        return false;

//    GLuint nFontHeight = assetManger.getFontItem(fontIndex).height;
//    ELEMENT_FONT elementFont(fontIndex,nFontHeight, assetManger.getFontItem(fontIndex).width);

    ELEMENT_GFX elementGFX;

    //-------------------------------------
    // Init Static elements
    //-------------------------------------
    // sets the Static default font
    // this font is also used for all other controls ... for now..
    // create the controls font
//    if (!assetManger.getFont("times new roman",16, 8, FW_BOLD, FALSE, fontIndex))
//        return false;

//    nFontHeight = assetManger.getFontItem(fontIndex).height;
//    ELEMENT_FONT elementFont2(fontIndex,nFontHeight, assetManger.getFontItem(fontIndex).width);
    ELEMENT_FONT elementFont2;

    elementFontVec.push_back(elementFont2);
    elementGFXvec.clear();

    controlGFX.nControlType =  ControlUI::STATIC;
    controlGFX.elementsGFXvec = elementGFXvec;
    controlGFX.elementsFontVec = elementFontVec;
    m_defaultControlsGFX.push_back(controlGFX);

    //-------------------------------------
    // Init Button elements
    //-------------------------------------
    // loads the button default texture
    if (!initButtonControlGFX(assetManger, ControlUI::BUTTON, "data/textures/button.png", Rect(0, 0, 256, 128),  Rect(0, 128, 256, 256), elementFontVec))
        return false;

    //-------------------------------------
    // Init CheckBox elements
    //-------------------------------------
    if (!initButtonControlGFX(assetManger, ControlUI::CHECKBOX, "data/textures/tex.dds", Rect(0, 54, 27, 81 ), Rect(27, 54, 54, 81 ), elementFontVec))
        return false;

    //-------------------------------------
    // Init RadioButton elements
    //-------------------------------------
    if (!initButtonControlGFX(assetManger, ControlUI::RADIOBUTTON, "data/textures/tex.dds", Rect(54, 54, 81, 81), Rect(81, 54, 108, 81), elementFontVec))
        return false;

    //-------------------------------------
    // Init ComboBox elements
    //-------------------------------------
    elementGFXvec.clear();
    textureIndex = assetManger.getTexture("data/textures/tex.dds");

    //-------------------------------------
    // ComboBox - Main
    //-------------------------------------
    elementGFXvec.emplace_back(textureIndex, Rect(7, 81, 247, 123), Rect(7, 81, 247, 123));

    //-------------------------------------
    // ComboBox - Button
    //-------------------------------------
    elementGFXvec.emplace_back(textureIndex, Rect(98, 189, 151, 238), Rect(98, 189, 151, 238));

    //-------------------------------------
    // ComboBox - Dropdown
    //-------------------------------------
    elementGFXvec.emplace_back(textureIndex, Rect(13, 123, 241, 160), Rect(13, 123, 241, 160));

    //-------------------------------------
    // ComboBox - Selection
    //-------------------------------------
    elementGFXvec.emplace_back(textureIndex, Rect(12, 163, 239, 183), Rect(12, 163, 239, 183));

    // create the ComboBox control default GFX and adds to the vector
    m_defaultControlsGFX.emplace_back(ControlUI::COMBOBOX, elementGFXvec, elementFontVec);

    //-------------------------------------
    // Init ScrollBar elements
    //-------------------------------------
    elementGFXvec.clear();

    //-------------------------------------
    // ScrollBar - Track
    //-------------------------------------
    int nScrollBarStartX = 196;
    int nScrollBarStartY = 191;
    elementGFXvec.emplace_back(textureIndex, Rect(nScrollBarStartX + 0, nScrollBarStartY + 21, nScrollBarStartX + 22, nScrollBarStartY + 32),
                               Rect(nScrollBarStartX + 0, nScrollBarStartY + 21, nScrollBarStartX + 22, nScrollBarStartY + 32));

    //-------------------------------------
    // ScrollBar - Up Arrow
    //-------------------------------------
    elementGFXvec.emplace_back(textureIndex, Rect(nScrollBarStartX + 0, nScrollBarStartY + 1, nScrollBarStartX + 22, nScrollBarStartY + 21),
                               Rect(nScrollBarStartX + 0, nScrollBarStartY + 1, nScrollBarStartX + 22, nScrollBarStartY + 21));

    //-------------------------------------
    // ScrollBar - Down Arrow
    //-------------------------------------
    elementGFXvec.emplace_back(textureIndex, Rect(nScrollBarStartX + 0, nScrollBarStartY + 32, nScrollBarStartX + 22, nScrollBarStartY + 53),
                               Rect(nScrollBarStartX + 0, nScrollBarStartY + 32, nScrollBarStartX + 22, nScrollBarStartY + 53));

    //-------------------------------------
    // ScrollBar - Button
    //-------------------------------------
    elementGFXvec.emplace_back(textureIndex, Rect(220, 192, 238, 234), Rect(220, 192, 238, 234));

    // create the ScrollBar control default GFX and adds to the vector
    m_defaultControlsGFX.emplace_back(ControlUI::SCROLLBAR, elementGFXvec, elementFontVec);

    //-------------------------------------
    // Init ListBox elements
    //-------------------------------------
    elementGFXvec.clear();

    //-------------------------------------
    // ListBox - Main
    //-------------------------------------
    elementGFXvec.emplace_back(textureIndex, Rect( 13, 123, 241, 160), Rect( 13, 123, 241, 160));

    //-------------------------------------
    //ListBox - Selection
    //-------------------------------------
    elementGFXvec.emplace_back(textureIndex, Rect( 16, 166, 240, 183), Rect( 16, 166, 240, 183));

    // create the ListBox control default GFX and adds to the vector
    m_defaultControlsGFX.emplace_back(ControlUI::LISTBOX, elementGFXvec, elementFontVec);

    //-------------------------------------
    // Init Slider elements
    //-------------------------------------
    elementGFXvec.clear();

    //-------------------------------------
    // Slider - Track
    //-------------------------------------
    elementGFXvec.emplace_back(textureIndex, Rect( 1, 187, 93, 228), Rect( 1, 187, 93, 228));

    //-------------------------------------
    // Slider - Button
    //-------------------------------------
    elementGFXvec.emplace_back(textureIndex, Rect( 151, 193, 192, 234), Rect( 151, 193, 192, 234));

    // create the Slider control default GFX and adds to the vector
    m_defaultControlsGFX.emplace_back(ControlUI::SLIDER, elementGFXvec, elementFontVec);

    //-------------------------------------
    // EditBox
    //-------------------------------------
    elementGFXvec.clear();

    // Element assignment:
    //   0 - text area
    //   1 - top left border
    //   2 - top border
    //   3 - top right border
    //   4 - left border
    //   5 - right border
    //   6 - lower left border
    //   7 - lower border
    //   8 - lower right border

    // Assign the style
    elementGFXvec.emplace_back(textureIndex, Rect( 14, 90, 241, 113), Rect( 14, 90, 241, 113));

    elementGFXvec.emplace_back(textureIndex, Rect( 8, 82, 14, 90 ), Rect( 8, 82, 14, 90 ));

    elementGFXvec.emplace_back(textureIndex, Rect( 14, 82, 241, 90 ), Rect( 14, 82, 241, 90 ));

    elementGFXvec.emplace_back(textureIndex, Rect( 241, 82, 246, 90 ), Rect( 241, 82, 246, 90 ));

    elementGFXvec.emplace_back(textureIndex, Rect( 8, 90, 14, 113 ), Rect( 8, 90, 14, 113 ));

    elementGFXvec.emplace_back(textureIndex, Rect( 241, 90, 246, 113 ), Rect( 241, 90, 246, 113 ));

    elementGFXvec.emplace_back(textureIndex, Rect( 8, 113, 14, 121 ), Rect( 8, 113, 14, 121 ));

    elementGFXvec.emplace_back(textureIndex, Rect( 14, 113, 241, 121 ), Rect( 14, 113, 241, 121 ));

    elementGFXvec.emplace_back(textureIndex, Rect( 241, 113, 246, 121 ), Rect( 241, 113, 246, 121 ));

    // create the Slider control default GFX and adds to the vector
    m_defaultControlsGFX.emplace_back(ControlUI::EDITBOX, elementGFXvec, elementFontVec);

    return true;
}

//-----------------------------------------------------------------------------
// Name : initControlGFX ()
//-----------------------------------------------------------------------------
bool DialogUI::initButtonControlGFX(AssetManager &assetManger, ControlUI::CONTROLS controlType,std::string texturePath,Rect rcTexture, Rect rcTexMouseOver, std::vector<ELEMENT_FONT>& elementFontVec)
{
    GLuint textureIndex = assetManger.getTexture(texturePath);
    if (textureIndex == NO_TEXTURE)
        return false;

    std::vector<ELEMENT_GFX>  elementGFXvec;
    // add the main button element
    elementGFXvec.emplace_back(textureIndex,
                               rcTexture, // sets what parts of the texture to use for the button
                               rcTexture);

    // add the mouse over button element
    elementGFXvec.emplace_back(textureIndex,
                               rcTexMouseOver,
                               rcTexMouseOver);

    // create the button control default GFX and adds to the vector
    m_defaultControlsGFX.emplace_back(controlType, elementGFXvec, elementFontVec);
}

//-----------------------------------------------------------------------------
// Name : OnRender ()
// Desc : renders the dialog and all of his controls
//-----------------------------------------------------------------------------
bool DialogUI::OnRender(float fElapsedTime, glm::vec3 vPos, Sprite& sprite, AssetManager& assetManger)
{
    GLuint textureName = NO_TEXTURE;

    if (!m_bVisible)
        return true;

    if (!m_texturePath.empty())
    {
        textureName = assetManger.getTexture(m_texturePath);
        if (textureName == NO_TEXTURE)
            return false;
    }

    sprite.AddTintedTexturedQuad(Rect(m_x, m_y, m_width, m_height), m_dialogColor, textureName);

    if (m_bCaption)
        sprite.AddTintedQuad(m_rcCaptionBox, glm::vec4(0.78125f, 1.0f, 0.0f, 1.0f));

    bool drawFocusedControl = false;

    for(GLuint i = 0; i < m_Controls.size(); i++)
    {
//        if (m_Controls[i] != s_pControlFocus)
//            m_Controls[i]->Render(assetManger);
    }

//    if (drawFocusedControl)
//            s_pControlFocus->Render(assetManger);

//    LPD3DXFONT pFont = assetManger.getFontPtr(0);
//    if (pFont)
//    {
//        if (m_bCaption)
//            pFont->DrawTextA(pTopSprite, m_captionText, -1, &m_rcCaptionBox, DT_LEFT, d3d::WHITE);
//    }

    return true;
}

//-----------------------------------------------------------------------------
// Name : MsgProc ()
// Desc : process messages that were sent to the dialog
//-----------------------------------------------------------------------------
bool DialogUI::MsgProc(GLuint uMsg, Timer* timer , bool windowed)
{
//    bool bHandled = false;

//    if (!m_bVisible)
//        return false;

//    INPUT_STATE curInputState;

//    // needs to be calcualted in linux
//    curInputState.bDoubleClick = uMsg == WM_LBUTTONDBLCLK;
//    curInputState.bCtrl = (wParam & MK_CONTROL) == MK_CONTROL;
//    curInputState.bShift = (wParam & MK_SHIFT) == MK_SHIFT;

//    // also needs to be calcualted
//    UINT uLines;
//    SystemParametersInfo( SPI_GETWHEELSCROLLLINES, 0, &uLines, 0 );
//    curInputState.nScrollAmount = int( ( short )HIWORD( wParam ) ) / WHEEL_DELTA * uLines;


//    // If a control is in focus, it belongs to this dialog, and it's enabled, then give
//    // it the first chance at handling the message.
//    if( s_pControlFocus &&
//        s_pControlFocus->getParentDialog() == this &&
//        s_pControlFocus->getEnabled() )
//    {
//        // If the control MsgProc handles it, then we don't.
//        if( s_pControlFocus->MsgProc( uMsg, wParam, lParam ) )
//            return true;
//    }

//    switch (uMsg)
//    {

//    // ConfigureNotify
//    case WM_SIZE:
//    case WM_MOVE:
//        {
//            // Handle sizing and moving messages so that in case the mouse cursor is moved out
//            // of an UI control because of the window adjustment, we can properly
//            // unhighlight the highlighted control.
//            POINT pt = {-1, -1};
//            OnMouseMove(pt);
//        }break;

//    // ButtonPress
//    // ButtonRelease
//    case WM_KEYDOWN:
//    case WM_SYSKEYDOWN:
//    case WM_KEYUP:
//    case WM_SYSKEYUP:
//        {
//            // If a control is in focus, it belongs to this dialog, and it's enabled, then give
//            // it the first chance at handling the message.
//            if( s_pControlFocus && s_pControlFocus->getParentDialog() == this &&
//                s_pControlFocus->getEnabled() )
//            {
//                if( s_pControlFocus->HandleKeyboard( hWnd, uMsg, wParam, lParam ) )
//                    return true;
//            }

//        }break;

//    // needs to be calcualted in linux
//    case WM_RBUTTONDBLCLK:
//        {
//            POINT mousePoint;

//            GetCursorPos(&mousePoint);
//            if (windowed)
//                ScreenToClient(hWnd,&mousePoint);

//            mousePoint.x -= m_x;
//            mousePoint.y -= m_y;

//            if( m_bCaption )
//                mousePoint.y -= m_nCaptionHeight;

//            CControlUI* pControl = getControlAtPoint( mousePoint );
//            if( pControl != NULL && pControl->getEnabled() )
//            {
//                RemoveControl( pControl->getID() );
//            }
//        }break;

//    // ButtonPress
//    // MotionNotify??
//    case WM_MOUSEMOVE:
//    case WM_LBUTTONDOWN:
//    case WM_LBUTTONUP:
//    case WM_MBUTTONDOWN:
//    case WM_MBUTTONUP:
//    case WM_RBUTTONDOWN:
//    case WM_RBUTTONUP:
//    case WM_XBUTTONDOWN:
//    case WM_XBUTTONUP:
//    case WM_LBUTTONDBLCLK:
//    case WM_MBUTTONDBLCLK:
//    //case WM_RBUTTONDBLCLK:
//    case WM_XBUTTONDBLCLK:
//    case WM_MOUSEWHEEL:
//        {
//            //next let controls handle the mouse message
//            POINT mousePoint;
//            GetCursorPos(&mousePoint);
//            if (windowed)
//                ScreenToClient(hWnd,&mousePoint);
//            mousePoint.x -= m_x;
//            mousePoint.y -= m_y + getCaptionHeight();

//            // If a control is in focus, it belongs to this dialog, and it's enabled, then give
//            // it the first chance at handling the message.
//            if( s_pControlFocus && s_pControlFocus->getParentDialog() == this &&
//                s_pControlFocus->getEnabled() )
//            {
//                if( s_pControlFocus->HandleMouse( hWnd, uMsg, mousePoint, curInputState, timer ) )
//                    return true;
//            }

//            for (UINT i = 0; i < m_Controls.size(); i++)
//            {
//                if (m_Controls[i]->HandleMouse(hWnd,uMsg,mousePoint, curInputState, timer))
//                {
//                    bHandled = true;
//                    break;
//                }
//            }

//            //bHandled = m_Controls[0]->HandleMouse(hWnd,uMsg,mousePoint, wParam, lParam,timer);
//            if (bHandled)
//                return bHandled;

//        }break;

//    }

//    // Mouse not over any controls in this dialog, if there was a control
//    // which had focus it just lost it
//    if( uMsg == WM_LBUTTONDOWN && s_pControlFocus &&
//        s_pControlFocus->getParentDialog() == this )
//    {
//        s_pControlFocus->OnFocusOut();
//        s_pControlFocus = NULL;
//    }

//    // the message was not for any of the controls
//    // lets the dialog handle the message
//    switch(uMsg)
//    {
//    // MotionNotify
//    case WM_MOUSEMOVE:
//        {
//            // check if we need to highlight a control as the mouse is over it
//            POINT mousePoint;
//            GetCursorPos(&mousePoint);
//            if (windowed)
//                ScreenToClient(hWnd,&mousePoint);
//            //mousePoint.y -=  m_rcCaptionBox.bottom;
//// 			mousePoint.x -= m_x;
//// 			mousePoint.y -= m_y;

//            OnMouseMove(mousePoint);

//            // return false to allow the message to be handled by app for mouse camera
//            return false;
//        }break;

//    case WM_LBUTTONDOWN:
//        {
//            GetCursorPos(&m_startDragPos);
//            if (windowed)
//                ScreenToClient(hWnd,&m_startDragPos);

//            if (PtInRect(&m_rcCaptionBox, m_startDragPos))
//            {
//                m_bDrag = true;
//                SetCapture(hWnd);
//                return true;
//            }
//        }break;

//    // ButtonRelease
//    case WM_LBUTTONUP:
//        {
//            if (m_bDrag)
//            {
//                m_bDrag = false;
//                ReleaseCapture();
//                return true;
//            }
//        }break;

//    case WM_RBUTTONDOWN:
//        {
//            SetCapture( m_hWnd );

//            POINT mousePoint;
//            CControlUI* pCurSelectedControl = nullptr;

//            GetCursorPos(&mousePoint);
//            if (windowed)
//                ScreenToClient(hWnd,&mousePoint);

//            mousePoint.x -= getLocation().x;
//            mousePoint.y -= getLocation().y + getCaptionHeight();

//            pCurSelectedControl = getControlAtPoint(mousePoint);

//            if (pCurSelectedControl != nullptr)
//                m_controlRightClkSig(pCurSelectedControl);

//        }break;
//    }

    return false;
}

//-----------------------------------------------------------------------------
// Name : connectToControlRightClicked
//-----------------------------------------------------------------------------
void DialogUI::connectToControlRightClicked(const signal_controlClicked::slot_type& subscriber)
{
    m_controlRightClkSig.connect(subscriber);
}

//-----------------------------------------------------------------------------
// Name : ClearRadioButtonGruop
// Desc : Clears the selection from the selected radio button group
//-----------------------------------------------------------------------------
void DialogUI::ClearRadioButtonGruop(GLuint nButtonGroup)
{
//    for (GLuint i = 0; i < m_Controls.size(); i++)
//    {
//        if (m_Controls[i]->getType() == ControlUI::CONTROLS::RADIOBUTTON)
//        {
//            RadioButtonUI* curRadioButton = (RadioButtonUI*)m_Controls[i];
//            if (curRadioButton->getButtonGroup() == nButtonGroup)
//                curRadioButton->setChecked(false);
//        }
//    }
}

//-----------------------------------------------------------------------------
// Name : OnMouseMove
// Desc : being called each time the mouse is moved
//		  check if the mouse is over a control
//		  if it is highlight that control.
//-----------------------------------------------------------------------------
void DialogUI::OnMouseMove(Point pt)
{
//    if (m_bDrag)
//    {
//        int dx = pt.x - m_startDragPos.x;
//        int dy = pt.y - m_startDragPos.y;

//        //TODO: use setLocation instead of manually adding the values
//        //setLocation(m_x + dx, m_y + dy);
//        m_x += dx;
//        m_y += dy;
//        UpdateRects();

//        m_startDragPos = pt;
//        return;
//    }

//    pt.x -= m_x;
//    pt.y -= m_y;
//    pt.y -= getCaptionHeight();

//    ControlUI* pControl = getControlAtPoint(pt);

//    // check if the current control was already highlighted
//    if (pControl == m_pMouseOverControl)
//        return;

//    // if there was a control highlighted restore it to normal
//    if (m_pMouseOverControl)
//        m_pMouseOverControl->onMouseLeave();

//    // sets the new mouseOverControl and highlight it
//    m_pMouseOverControl = pControl;
//    if (pControl)
//        pControl->onMouseEnter();

}

//-----------------------------------------------------------------------------
// Name : getControlAtPoint
// Desc : check if the given point is inside
//        one of the controls that is in the dialog
//		  and if found returns a pointer to that control
//-----------------------------------------------------------------------------
ControlUI* DialogUI::getControlAtPoint(Point pt)
{
    for (GLuint i = 0; i < m_Controls.size(); i++)
    {
        if (m_Controls[i]->getVisible() && m_Controls[i]->getEnabled())
        {
            if (m_Controls[i]->ContainsPoint(pt))
                return m_Controls[i];
        }
    }

    // if no control was found to have the point within it return null
    return nullptr;
}

//-----------------------------------------------------------------------------
// Name : addStatic
// Desc : adds a control of type static to the dialog
//-----------------------------------------------------------------------------
bool DialogUI::addStatic(int ID, const std::string& strText, int x, int y, int width, int height, StaticUI** ppStaticCreated/* = NULL*/, std::string strID /*= ""*/)
{
//    //initialized the static control
//    StaticUI* pStatic = new StaticUI(this ,ID, strText, x, y, width, height);

//    initControl(pStatic);

//    //add it to the controls vector
//    m_Controls.push_back(pStatic);
//    // add the control info the Definition Vector
//    m_defInfo.push_back( DEF_INFO(strID, ID) );

//    if (ppStaticCreated != NULL)
//        *ppStaticCreated = pStatic;

//    return true;
    return false;
}

//-----------------------------------------------------------------------------
// Name : addButton
// Desc : add a control of type button to the dialog
//-----------------------------------------------------------------------------
bool DialogUI::addButton(int ID, const std::string &strText, int x, int y, int width, int height, GLuint nHotkey, ButtonUI** ppButtonCreated/* = NULL*/, std::string strID /*= ""*/)
{
    //initialized the button control
    ButtonUI* pButton = new  ButtonUI(this ,ID, strText, x, y, width, height, nHotkey);

    initControl(pButton);

    //add it to the controls vector
    m_Controls.push_back(pButton);
    m_defInfo.push_back( DEF_INFO(strID, ID) );

    if (ppButtonCreated != NULL)
        *ppButtonCreated = pButton;

    return true;
}

//-----------------------------------------------------------------------------
// Name : addCheckBox()
// Desc : add a control of type checkBox to the dialog
//-----------------------------------------------------------------------------
//bool DialogUI::addCheckBox(int ID, int x, int y, int width, int height, GLuint nHotkey, CheckboxUI** ppCheckBoxCreated/* = NULL*/,  std::string strID /*= ""*/)
//{
//    //initialized the checkBox control
//    CheckboxUI* pCheckBox = new CheckboxUI(this, ID, x, y, width, height, nHotkey);

//    initControl(pCheckBox);

//    //add it to the controls vector
//    m_Controls.push_back(pCheckBox);
//    m_defInfo.push_back( DEF_INFO(strID, ID) );

//    if (ppCheckBoxCreated != NULL)
//        *ppCheckBoxCreated = pCheckBox;

//    return true;
//}

//-----------------------------------------------------------------------------
// Name : addRadioButton()
// Desc : add a control of type radio button to the dialog
//-----------------------------------------------------------------------------
//bool DialogUI::addRadioButton(int ID, int x, int y, int width, int height, GLuint nHotkey, GLuint nButtonGroup, RadioButtonUI** ppRadioButtonCreated/* = NULL*/ ,std::string strID /*= ""*/)
//{
//    RadioButtonUI* pRadioButton = new RadioButtonUI(this, ID, x, y, width, height, nHotkey, nButtonGroup);

//    initControl(pRadioButton);

//    //add it to the controls vector
//    m_Controls.push_back(pRadioButton);
//    m_defInfo.push_back( DEF_INFO(strID, ID) );

//    if (ppRadioButtonCreated != NULL)
//        *ppRadioButtonCreated = pRadioButton;

//    return true;
//}

//-----------------------------------------------------------------------------
// Name : addComboBox()
// Desc : add a control of type Combobox to the dialog
//-----------------------------------------------------------------------------
//bool DialogUI::addComboBox(int ID, LPCTSTR strText, int x, int y, int width, int height, GLuint nHotkey, ComboBoxUI** ppComboxCreated/* = NULL*/ , std::string strID /*= ""*/)
//{
//    ComboBoxUI* pComboBox = new ComboBoxUI(this, ID, strText, x, y, width, height, nHotkey);

//    initControl(pComboBox);

//    pComboBox->UpdateRects();

//    //add it to the controls vector
//    m_Controls.push_back(pComboBox);
//    m_defInfo.push_back( DEF_INFO(strID, ID) );

//    if (ppComboxCreated != NULL)
//        *ppComboxCreated = pComboBox;

//    return true;
//}

//-----------------------------------------------------------------------------
// Name : addListBox()
// Desc : add a control of type ListBox to the dialog
//-----------------------------------------------------------------------------
//bool DialogUI::addListBox(int ID, int x, int y, int width, int height, DWORD style/* = 0*/, ListBoxUI** ppListBoxCreated/* = NULL*/, std::string strID /*= ""*/)
//{
//    ListBoxUI* pListBox = new ListBoxUI(this, ID, x, y, width, height, style);

//    initControl(pListBox);
//    // !!! list box needs to update it's Rects on init
//    pListBox->UpdateRects();

//    //add it to the controls vector
//    m_Controls.push_back(pListBox);
//    m_defInfo.push_back( DEF_INFO(strID, ID) );

//    if (ppListBoxCreated != NULL)
//        *ppListBoxCreated = pListBox;

//    return true;
//}

//-----------------------------------------------------------------------------
// Name : addSlider()
// Desc : add a control of type Slider to the dialog
//-----------------------------------------------------------------------------
//bool DialogUI::addSlider( int ID, int x, int y, int width, int height, int min, int max, int nValue, SliderUI** ppSliderCreated/* = NULL*/, std::string strID /*= ""*/ )
//{
//    SliderUI* pSlider = new SliderUI(this, ID, x, y, width, height, min, max, nValue);

//    initControl(pSlider);
//    // !!! slider needs to update it's Rects on init
//    pSlider->UpdateRects();

//    //add it to the controls vector
//    m_Controls.push_back(pSlider);
//    m_defInfo.push_back( DEF_INFO(strID, ID) );

//    if (ppSliderCreated != NULL)
//        *ppSliderCreated = pSlider;

//    return true;
//}

//-----------------------------------------------------------------------------
// Name : addEditbox()
// Desc : add a control of type EditBox to the dialog
//-----------------------------------------------------------------------------
//bool DialogUI::addEditbox( int ID, LPCTSTR strText, int x, int y, int width, int height, Timer* timer, EditBoxUI** ppEditBoxCreated/* = NULL*/, std::string strID /*= ""*/)
//{
//    EditBoxUI* pEditBox = new EditBoxUI(this, ID, strText, x, y, width, height, timer);

//    initControl(pEditBox);
//    // !!! EditBox needs to update it's Rects on init
//    pEditBox->UpdateRects();

//    //add it to the controls vector
//    m_Controls.push_back(pEditBox);
//    m_defInfo.push_back( DEF_INFO(strID, ID) );

//    if (ppEditBoxCreated != NULL)
//        *ppEditBoxCreated = pEditBox;

//    return true;
//}

//-----------------------------------------------------------------------------
// Name : addStaticFromFile
// Desc : loads form file  a control of type static to the dialog
//-----------------------------------------------------------------------------
bool DialogUI::addStaticFromFile(std::istream& InputFIle, StaticUI** ppStaticCreated/* = NULL*/)
{
    //initialized the static control from file
    StaticUI* pStatic = new StaticUI(InputFIle);

    initControl(pStatic);

    //add it to the controls vector
    m_Controls.push_back(pStatic);

    if (ppStaticCreated != NULL)
        *ppStaticCreated = pStatic;

    return true;
}

//-----------------------------------------------------------------------------
// Name : addButtonFromFile
// Desc : loads from file a control of type Button to the dialog
//-----------------------------------------------------------------------------
bool DialogUI::addButtonFromFile(std::istream& InputFIle, ButtonUI** ppButtonCreated /*= NULL*/)
{
    //initialized the button control from file
    ButtonUI* pButton = new  ButtonUI(InputFIle);

    initControl(pButton);

    //add it to the controls vector
    m_Controls.push_back(pButton);

    if (ppButtonCreated != NULL)
        *ppButtonCreated = pButton;

    return true;
}

//-----------------------------------------------------------------------------
// Name : addCheckBoxFromFile()
// Desc : loads from file a control of type CheckBox to the dialog
//-----------------------------------------------------------------------------
//bool DialogUI::addCheckBoxFromFile(std::istream& InputFIle, CheckboxUI** ppCheckBoxCreated /* = NULL */)
//{
//    //initialized the checkBox control from file
//    CheckboxUI* pCheckBox = new CheckboxUI(InputFIle);

//    initControl(pCheckBox);

//    //add it to the controls vector
//    m_Controls.push_back(pCheckBox);

//    if (ppCheckBoxCreated != NULL)
//        *ppCheckBoxCreated = pCheckBox;

//    return true;
//}

//-----------------------------------------------------------------------------
// Name : addRadioButtonFromFile()
// Desc : loads from file a control of type RadioButton to the dialog
//-----------------------------------------------------------------------------
//bool DialogUI::addRadioButtonFromFile(std::istream& InputFIle, RadioButtonUI** ppRadioButtonCreated /* = NULL */)
//{
//    RadioButtonUI* pRadioButton = new RadioButtonUI(InputFIle);

//    initControl(pRadioButton);

//    //add it to the controls vector
//    m_Controls.push_back(pRadioButton);

//    if (ppRadioButtonCreated != NULL)
//        *ppRadioButtonCreated = pRadioButton;

//    return true;
//}

//-----------------------------------------------------------------------------
// Name : addComboBoxFromFile()
// Desc : loads from file a control of type ComboBox to the dialog
//-----------------------------------------------------------------------------
//bool DialogUI::addComboBoxFromFile(std::istream& InputFIle, ComboBoxUI** ppComboxCreated /* = NULL */)
//{
//    ComboBoxUI* pComboBox = new ComboBoxUI(InputFIle);

//    initControl(pComboBox);

//    pComboBox->UpdateRects();

//    //add it to the controls vector
//    m_Controls.push_back(pComboBox);

//    if (ppComboxCreated != NULL)
//        *ppComboxCreated = pComboBox;

//    return true;
//}

//-----------------------------------------------------------------------------
// Name : addListBoxFromFile()
// Desc : loads from file a control of type ListBox to the dialog
//-----------------------------------------------------------------------------
//bool DialogUI::addListBoxFromFile(std::istream& InputFIle, ListBoxUI** ppListBoxCreated /* = NULL */)
//{
//    ListBoxUI* pListBox = new ListBoxUI(InputFIle);

//    initControl(pListBox);
//    // !!! list box needs to update it's Rects on init
//    pListBox->UpdateRects();

//    //add it to the controls vector
//    m_Controls.push_back(pListBox);

//    if (ppListBoxCreated != NULL)
//        *ppListBoxCreated = pListBox;

//    return true;
//}

//-----------------------------------------------------------------------------
// Name : addSliderFromFile()
// Desc : loads from file a control of type Slider to the dialog
//-----------------------------------------------------------------------------
//bool DialogUI::addSliderFromFile(std::istream& InputFIle, SliderUI** ppSliderCreated /* = NULL */)
//{
//    SliderUI* pSlider = new SliderUI(InputFIle);

//    initControl(pSlider);
//    // !!! slider needs to update it's Rects on init
//    pSlider->UpdateRects();

//    //add it to the controls vector
//    m_Controls.push_back(pSlider);

//    if (ppSliderCreated != NULL)
//        *ppSliderCreated = pSlider;

//    return true;
//}

//-----------------------------------------------------------------------------
// Name : addEditBoxFromFile()
// Desc : loads from file a control of type EditBox to the dialog
//-----------------------------------------------------------------------------
//bool DialogUI::addEditBoxFromFile(std::istream& InputFIle, Timer* timer, EditBoxUI** ppEditBoxCreated /* = NULL */)
//{
//    EditBoxUI* pEditBox = new EditBoxUI(InputFIle,timer);

//    initControl(pEditBox);
//    // !!! EditBox needs to update it's Rects on init
//    pEditBox->UpdateRects();

//    //add it to the controls vector
//    m_Controls.push_back(pEditBox);

//    if (ppEditBoxCreated != NULL)
//        *ppEditBoxCreated = pEditBox;

//    return true;
//}

//-----------------------------------------------------------------------------
// Name : RemoveControl()
//-----------------------------------------------------------------------------
void DialogUI::RemoveControl(int ID)
{
    for (GLuint i = 0; i < m_Controls.size(); i++)
    {
        if (m_Controls[i]->getID() == ID)
        {
            // clear the focus reference to this control
            if (s_pControlFocus == m_Controls[i])
                ClearFocus();

            // clear the mouseOver reference to this control
            if (m_pMouseOverControl == m_Controls[i])
                m_pMouseOverControl = nullptr;

            // Deletes the control and removes it from the vecotr
            delete m_Controls[i];
            m_Controls[i] = nullptr;

            m_Controls.erase(m_Controls.begin() + i);

            // remove the reference from the def vector to this control
            for (GLuint j = 0; j < m_defInfo.size(); j++)
            {
                if (m_defInfo[j].controlID == ID )
                {
                    m_defInfo.erase(m_defInfo.begin() + j);
                    break;
                }
            }

            return;
        }
    }
}

//-----------------------------------------------------------------------------
// Name : RemoveAllControls
//-----------------------------------------------------------------------------
void DialogUI::RemoveAllControls()
{
    if( s_pControlFocus && s_pControlFocus->getParentDialog() == this )
        s_pControlFocus = nullptr;

    m_pMouseOverControl = nullptr;

    for( GLuint i = 0; i < m_Controls.size(); i++ )
    {
        delete m_Controls[i];
        m_Controls[i] = nullptr;
    }

    m_Controls.clear();
}

//-----------------------------------------------------------------------------
// Name : initControl()
// Desc : initialize a control by giving it the default control settings
//-----------------------------------------------------------------------------
//TODO: remove the setParent command form this function
bool DialogUI::initControl(ControlUI* pControl)
{
    if (pControl == NULL)
        return false;

    // look for a default graphics for the control
    for (GLuint i = 0; i < m_defaultControlsGFX.size(); i++)
    {
        if (m_defaultControlsGFX[i].nControlType == pControl->getType())
        {
            pControl->setControlGFX(m_defaultControlsGFX[i].elementsGFXvec);
            pControl->setControlFonts(m_defaultControlsGFX[i].elementsFontVec);
            break;
        }
    }

    //sets the check box parent .. this dialog
    pControl->setParent(this);

    return pControl->onInit();
}


//-----------------------------------------------------------------------------
// Name : UpdateControlDefText()
//-----------------------------------------------------------------------------
void DialogUI::UpdateControlDefText(std::string&& strDefText, int controlID)
{
    for (GLuint i = 0; i < m_defInfo.size(); i++)
        if (m_defInfo[i].controlID == controlID)
        {
            m_defInfo[i].controlIDText = strDefText;
            break;
        }
}

//-----------------------------------------------------------------------------
// Name : setSize()
//-----------------------------------------------------------------------------
void DialogUI::setSize(GLuint width, GLuint height)
{
    m_width = width;
    m_height = height;
    UpdateRects();
}

//-----------------------------------------------------------------------------
// Name : setLocation()
//-----------------------------------------------------------------------------
void DialogUI::setLocation(int x, int y)
{
    m_x = x;
    m_y = y;
    UpdateRects();
}

//-----------------------------------------------------------------------------
// Name : setVisible()
//-----------------------------------------------------------------------------
void DialogUI::setVisible(bool bVisible)
{
    m_bVisible = bVisible;
}

//-----------------------------------------------------------------------------
// Name : setCaption()
//-----------------------------------------------------------------------------
void DialogUI::setCaption(bool bCaption)
{
    m_bCaption = bCaption;
}

//-----------------------------------------------------------------------------
// Name : UpdateRects()
// Desc : Update the dialog bounding box and it's caption box every tine the dialog
//		  is moved
//-----------------------------------------------------------------------------
void DialogUI::UpdateRects()
{
    m_rcBoundingBox = Rect(m_x ,m_y, m_x + m_width, m_y + m_height);
    m_rcCaptionBox = Rect( m_x, m_y, m_x + m_width, m_y + m_nCaptionHeight);
}

//-----------------------------------------------------------------------------
// Name : getLocation()
//-----------------------------------------------------------------------------
Point DialogUI::getLocation()
{
    Point loc = {m_x,m_y};
    return loc;
}

//-----------------------------------------------------------------------------
// Name : getWidth()
//-----------------------------------------------------------------------------
GLuint DialogUI::getWidth()
{
    return m_width;
}

//-----------------------------------------------------------------------------
// Name : getHeight()
//-----------------------------------------------------------------------------
GLuint DialogUI::getHeight()
{
    return m_height;
}

//-----------------------------------------------------------------------------
// Name : getCaptionStartPoint()
//-----------------------------------------------------------------------------
long DialogUI::getCaptionHeight()
{
    return m_rcCaptionBox.bottom - m_rcCaptionBox.top;
}


//-----------------------------------------------------------------------------
// Name : RequestFocus()
//-----------------------------------------------------------------------------
void DialogUI::RequestFocus( ControlUI* pControl)
{
    if( s_pControlFocus == pControl )
        return;

    if( !pControl->CanHaveFocus() )
        return;

    if( s_pControlFocus )
        s_pControlFocus->OnFocusOut();

    pControl->OnFocusIn();
    s_pControlFocus = pControl;
}

//-----------------------------------------------------------------------------
// Name : ClearFocus
//-----------------------------------------------------------------------------
void DialogUI::ClearFocus()
{
    if( s_pControlFocus )
    {
        s_pControlFocus->OnFocusOut();
        s_pControlFocus = NULL;
    }

    //ReleaseCapture();
}

//-----------------------------------------------------------------------------
// Name : SaveDilaogToFile
// Desc : Saves the dialog and all of his contorls to file
//-----------------------------------------------------------------------------
bool DialogUI::SaveDilaogToFile(std::string&& FileName, GLulong curControlID)
{
    std::ofstream saveFile,defFile;
    std::string defFileName = FileName;

    defFileName.resize(defFileName.size() - 4); //deleting the file extension (.xxx)
    defFileName = defFileName + "Def.h";

    defFile.open(defFileName.c_str());

    for (GLuint i = 0; i < m_Controls.size(); i++)
    {
        defFile << "#define " << m_defInfo[i].controlIDText << " " << m_defInfo[i].controlID << "\n";
    }

    defFile.close();

    saveFile.open(FileName);

    saveFile << m_width << "| Dialog Width" << "\n";
    saveFile << m_height << "| Dialog Height" << "\n";
    saveFile << m_nCaptionHeight << "| Dialog Caption Height" << "\n";
    saveFile << m_captionText << "| Dialog Caption Text" << "\n";
    saveFile << curControlID << "| Current Control ID" << "\n";

    saveFile << "/////////////////////////////////////////////////////////////////////////////\n";

    for (GLuint i = 0; i < m_Controls.size(); i++)
    {
        m_Controls[i]->SaveToFile(saveFile);

        saveFile << "/////////////////////////////////////////////////////////////////////////////\n";
    }

    saveFile.close();
    return true;
}

//-----------------------------------------------------------------------------
// Name : LoadDialogFromFile
// Desc : loads the dialog and all of his controls from file
//-----------------------------------------------------------------------------
GLulong DialogUI::LoadDialogFromFile(std::string&& FileName, Timer* timer)
{
    return 0;
//    std::ifstream inputFile;
//    GLuint controlType;

//    // clear controls on the dialog
//    RemoveAllControls();

//    inputFile.open(FileName, std::ifstream::in);

//    //load the Dialog parameters
//    inputFile >> m_width;
//    inputFile.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); //skips to next line
//    inputFile >> m_height;
//    inputFile.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); //skips to next line
//    inputFile >> m_nCaptionHeight;
//    inputFile.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); //skips to next line

//    std::string captionText;
//    std::getline(inputFile, captionText);
//    captionText = captionText.substr(0, captionText.find('|') );
//    strcpy_s(m_captionText, MAX_PATH, captionText.c_str() );

//    ULONG curControlID = 0;
//    inputFile >> curControlID;
//    inputFile.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); //skips to next line

//    inputFile.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); //skips to next line

//    do
//    {
//        inputFile >> controlType;
//        inputFile.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); //skips to next line
//        if (inputFile.eof() )
//            break;

//        switch(controlType)
//        {
//        case CControlUI::STATIC:
//            {
//                addStaticFromFile(inputFile);
//            }break;

//        case CControlUI::BUTTON:
//            {
//                addButtonFromFile(inputFile);
//            }break;

//        case CControlUI::CHECKBOX:
//            {
//                addCheckBoxFromFile(inputFile);
//            }break;

//        case CControlUI::COMBOBOX:
//            {
//                addComboBoxFromFile(inputFile);
//            }break;

//        case CControlUI::EDITBOX:
//            {
//                addEditBoxFromFile(inputFile, timer);
//            }break;

//        case CControlUI::LISTBOX:
//            {
//                addListBoxFromFile(inputFile);
//            }break;

//        case CControlUI::RADIOBUTTON:
//            {
//                addRadioButtonFromFile(inputFile);
//            }break;

//        case CControlUI::SLIDER:
//            {
//                addSliderFromFile(inputFile);
//            }
//        }

//        inputFile.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); //skips to next line
//    }while(!inputFile.eof());

//    inputFile.close();

//    m_defInfo.clear();

//    std::ifstream inputDefFile;

//    std::string defFileName;
//    std::string strID, sID;
//    int ID;

//    defFileName = FileName;
//    defFileName.resize(defFileName.size() - 4); //deleting the file extension (.xxx)
//    defFileName = defFileName + "Def.h";

//    inputDefFile.open(defFileName);

//    do
//    {
//        inputDefFile >> strID;

//        if (inputDefFile.eof())
//            break;

//        inputDefFile >> strID;
//        inputDefFile >> sID;
//        ID = std::stoi(sID);

//        m_defInfo.push_back( DEF_INFO(strID, ID) );

//        inputDefFile.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
//    }while(!inputDefFile.eof());

//    inputDefFile.close();

//    return curControlID;
}

//-----------------------------------------------------------------------------
// Name : getControl
// Desc : searches for a control base on ID ONLY
//-----------------------------------------------------------------------------
ControlUI* DialogUI::getControl( int ID )
{
    // Try to find the control with the given ID
    for( GLuint i = 0; i < m_Controls.size(); i++ )
    {
        ControlUI* pControl = m_Controls[i];

        if( pControl->getID() == ID )
        {
            return pControl;
        }
    }

    // Not found
    return NULL;
}


//-----------------------------------------------------------------------------
// Name : getControl
// Desc : searches for a control base on ID and Control Type
//-----------------------------------------------------------------------------
ControlUI* DialogUI::getControl( int ID, GLuint nControlType )
{
    // Try to find the control with the given ID
    for( GLuint i = 0; i < m_Controls.size(); i++ )
    {
        ControlUI* pControl = m_Controls[i];

        if( pControl->getID() == ID && pControl->getType() == nControlType )
        {
            return pControl;
        }
    }

    // Not found
    return NULL;
}

//-----------------------------------------------------------------------------
// Name : getStatic
//-----------------------------------------------------------------------------
//StaticUI* DialogUI::getStatic( int ID )
//{
//    return static_cast< CStaticUI* >(getControl( ID, ControlUI::STATIC ) );
//}

//-----------------------------------------------------------------------------
// Name : getButton
//-----------------------------------------------------------------------------
//ButtonUI* DialogUI::getButton( int ID )
//{
//    return static_cast< CButtonUI* > (getControl(ID, ControlUI::BUTTON) );
//}

//-----------------------------------------------------------------------------
// Name : getCheckBox
//-----------------------------------------------------------------------------
//CheckboxUI * DialogUI::getCheckBox( int ID )
//{
//    return static_cast< CCheckboxUI* > ( getControl(ID, ControlUI::CHECKBOX) );
//}

//-----------------------------------------------------------------------------
// Name : getRadioButton
//-----------------------------------------------------------------------------
//RadioButtonUI * DialogUI::getRadioButton ( int ID )
//{
//    return static_cast< CRadioButtonUI* > ( getControl(ID, ControlUI::RADIOBUTTON) );
//}

//-----------------------------------------------------------------------------
// Name : GetComboBox
//-----------------------------------------------------------------------------
//ComboBoxUI * DialogUI::getComboBox( int ID )
//{
//    return static_cast< CComboBoxUI* > (getControl(ID, ControlUI::COMBOBOX));
//}

//-----------------------------------------------------------------------------
// Name : GetSlider
//-----------------------------------------------------------------------------
//SliderUI * DialogUI::getSlider( int ID )
//{
//    return static_cast< CSliderUI* > (getControl(ID, ControlUI::SLIDER));
//}

//-----------------------------------------------------------------------------
// Name : GetEditBox
//-----------------------------------------------------------------------------
//EditBoxUI * DialogUI::getEditBox( int ID )
//{
//    return static_cast< CEditBoxUI* > (getControl(ID, ControlUI::EDITBOX));
//}

//-----------------------------------------------------------------------------
// Name : GetListBox
//-----------------------------------------------------------------------------
//ListBoxUI * DialogUI::getListBox( int ID )
//{
//    return static_cast< CListBoxUI* > (getControl(ID, ControlUI::LISTBOX));
//}

//-----------------------------------------------------------------------------
// Name : GetControlsNum
//-----------------------------------------------------------------------------
int DialogUI::getControlsNum()
{
    return m_Controls.size();
}

//-----------------------------------------------------------------------------
// Name : getControlIDText
//-----------------------------------------------------------------------------
const char* DialogUI::getControlIDText(int ID)
{
    for (GLuint i = 0; i <m_defInfo.size(); i++)
    {
        if (m_defInfo[i].controlID == ID)
            return m_defInfo[i].controlIDText.c_str();
    }

    return nullptr;
}

//-----------------------------------------------------------------------------
// Name : getVisible
//-----------------------------------------------------------------------------
bool DialogUI::getVisible()
{
    return m_bVisible;
}
