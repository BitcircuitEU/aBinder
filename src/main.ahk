;================================================================
; Instanz Settings
;================================================================
GroupAdd, GTASA, ahk_class Grand theft auto San Andreas ahk_exe gta_sa.exe
Listlines Off
SetBatchlines -1
#IfWinActive, ahk_group GTASA
#NoEnv
#MaxHotkeysPerInterval 99000000
#HotkeyInterval 99000000
#KeyHistory 0
#SingleInstance Force
ListLines Off
SendMode Input
SetWorkingDir, %A_ScriptDir%

;================================================================
; Includes
;================================================================
#Include %A_ScriptDir%\libs\JSON.ahk
#Include %A_ScriptDir%\libs\SocketIO.ahk
#Include %A_ScriptDir%\updateFunc.ahk
#Include %A_ScriptDir%\libs\SAMP.ahk

;================================================================
; Global Auth Vars
;================================================================
global Socket := new socketClass()
global CredentialsFile := A_ScriptDir . "\credentials.ini"
global Username
global Password

;================================================================
; Global Icon Vars & Icon Loader
;================================================================
iconsDir := A_ScriptDir "\icons"
IfNotExist, %iconsDir%
    FileCreateDir, %iconsDir%

; Icon URLs
novaIconURL := "https://nova-network.one/favicon.png"
loginIconURL := "https://img.icons8.com/?size=100&id=8094&format=png&color=ffffff"
logoutIconURL := "https://img.icons8.com/?size=100&id=Q1xkcFuVON39&format=png&color=ffffff"
closeIconURL := "https://img.icons8.com/?size=100&id=71200&format=png&color=ffffff"
minimizeIconURL := "https://img.icons8.com/?size=100&id=85496&format=png&color=ffffff"
homeIconURL := "https://img.icons8.com/?size=100&id=83326&format=png&color=ffffff"
keybindsIconURL := "https://img.icons8.com/?size=100&id=87753&format=png&color=ffffff"
textbindsIconURL := "https://img.icons8.com/?size=100&id=82775&format=png&color=ffffff"
posbindsIconURL := "https://img.icons8.com/?size=100&id=85049&format=png&color=ffffff"
frakbindsIconURL := "https://img.icons8.com/?size=100&id=115603&format=png&color=ffffff"
groupbindsIconURL := "https://img.icons8.com/?size=100&id=85369&format=png&color=ffffff"
overlaysIconURL := "https://img.icons8.com/?size=100&id=113299&format=png&color=ffffff"
settingsIconURL := "https://img.icons8.com/?size=100&id=82535&format=png&color=ffffff"

; Hover Icon URLs 
loginIconHoverURL := "https://img.icons8.com/?size=100&id=8094&format=png&color=a1a1a1"
logoutIconHoverURL := "https://img.icons8.com/?size=100&id=Q1xkcFuVON39&format=png&color=a1a1a1"
closeIconHoverURL := "https://img.icons8.com/?size=100&id=71200&format=png&color=a1a1a1"
minimizeIconHoverURL := "https://img.icons8.com/?size=100&id=85496&format=png&color=a1a1a1"
homeIconHoverURL := "https://img.icons8.com/?size=100&id=83326&format=png&color=a1a1a1"
keybindsIconHoverURL := "https://img.icons8.com/?size=100&id=87753&format=png&color=a1a1a1"
textbindsIconHoverURL := "https://img.icons8.com/?size=100&id=82775&format=png&color=a1a1a1"
posbindsIconHoverURL := "https://img.icons8.com/?size=100&id=85049&format=png&color=a1a1a1"
frakbindsIconHoverURL := "https://img.icons8.com/?size=100&id=115603&format=png&color=a1a1a1"
groupbindsIconHoverURL := "https://img.icons8.com/?size=100&id=85369&format=png&color=a1a1a1"
overlaysIconHoverURL := "https://img.icons8.com/?size=100&id=113299&format=png&color=a1a1a1"
settingsIconHoverURL := "https://img.icons8.com/?size=100&id=82535&format=png&color=a1a1a1"

; Icon Globals
global novaIconPath := iconsDir "\nova_icon.png"
global loginIconPath := iconsDir "\login_icon.png"
global logoutIconPath := iconsDir "\logout_icon.png"
global closeIconPath := iconsDir "\close_icon.png"
global minimizeIconPath := iconsDir "\minimize_icon.png"
global homeIconPath := iconsDir "\home_icon.png"
global keybindsIconPath := iconsDir "\keybinds_icon.png"
global textbindsIconPath := iconsDir "\textbinds_icon.png"
global posbindsIconPath := iconsDir "\posbinds_icon.png"
global frakbindsIconPath := iconsDir "\frakbinds_icon.png"
global groupbindsIconPath := iconsDir "\groupbinds_icon.png"
global overlaysIconPath := iconsDir "\overlays_icon.png"
global settingsIconPath := iconsDir "\settings_icon.png"

global loginIconHoverPath := iconsDir "\login_icon_hover.png"
global logoutIconHoverPath := iconsDir "\logout_icon_hover.png"
global closeIconHoverPath := iconsDir "\close_icon_hover.png"
global minimizeIconHoverPath := iconsDir "\minimize_icon_hover.png"
global homeIconHoverPath := iconsDir "\home_icon_hover.png"
global keybindsIconHoverPath := iconsDir "\keybinds_icon_hover.png"
global textbindsIconHoverPath := iconsDir "\textbinds_icon_hover.png"
global posbindsIconHoverPath := iconsDir "\posbinds_icon_hover.png"
global frakbindsIconHoverPath := iconsDir "\frakbinds_icon_hover.png"
global groupbindsIconHoverPath := iconsDir "\groupbinds_icon_hover.png"
global overlaysIconHoverPath := iconsDir "\overlays_icon_hover.png"
global settingsIconHoverPath := iconsDir "\settings_icon_hover.png"

; Download icons if not present
IfNotExist, %novaIconPath%
    URLDownloadToFile, %novaIconURL%, %novaIconPath%

; Normal Icons
IfNotExist, %loginIconPath%
    URLDownloadToFile, %loginIconURL%, %loginIconPath%
IfNotExist, %logoutIconPath%
    URLDownloadToFile, %logoutIconURL%, %logoutIconPath%
IfNotExist, %closeIconPath%
    URLDownloadToFile, %closeIconURL%, %closeIconPath%
IfNotExist, %minimizeIconPath%
    URLDownloadToFile, %minimizeIconURL%, %minimizeIconPath%
IfNotExist, %homeIconPath%
    URLDownloadToFile, %homeIconURL%, %homeIconPath%
IfNotExist, %keybindsIconPath%
    URLDownloadToFile, %keybindsIconURL%, %keybindsIconPath%
IfNotExist, %textbindsIconPath%
    URLDownloadToFile, %textbindsIconURL%, %textbindsIconPath%
IfNotExist, %posbindsIconPath%
    URLDownloadToFile, %posbindsIconURL%, %posbindsIconPath%
IfNotExist, %frakbindsIconPath%
    URLDownloadToFile, %frakbindsIconURL%, %frakbindsIconPath%
IfNotExist, %groupbindsIconPath%
    URLDownloadToFile, %groupbindsIconURL%, %groupbindsIconPath%
IfNotExist, %overlaysIconPath%
    URLDownloadToFile, %overlaysIconURL%, %overlaysIconPath%
IfNotExist, %settingsIconPath%
    URLDownloadToFile, %settingsIconURL%, %settingsIconPath%

; Hover Icons
IfNotExist, %loginIconHoverPath%
    URLDownloadToFile, %loginIconHoverURL%, %loginIconHoverPath%
IfNotExist, %logoutIconHoverPath%
    URLDownloadToFile, %logoutIconHoverURL%, %logoutIconHoverPath%
IfNotExist, %closeIconHoverPath%
    URLDownloadToFile, %closeIconHoverURL%, %closeIconHoverPath%
IfNotExist, %minimizeIconHoverPath%
    URLDownloadToFile, %minimizeIconHoverURL%, %minimizeIconHoverPath%
IfNotExist, %homeIconHoverPath%
    URLDownloadToFile, %homeIconHoverURL%, %homeIconHoverPath%
IfNotExist, %keybindsIconHoverPath%
    URLDownloadToFile, %keybindsIconHoverURL%, %keybindsIconHoverPath%
IfNotExist, %textbindsIconHoverPath%
    URLDownloadToFile, %textbindsIconHoverURL%, %textbindsIconHoverPath%
IfNotExist, %posbindsIconHoverPath%
    URLDownloadToFile, %posbindsIconHoverURL%, %posbindsIconHoverPath%
IfNotExist, %frakbindsIconHoverPath%
    URLDownloadToFile, %frakbindsIconHoverURL%, %frakbindsIconHoverPath%
IfNotExist, %groupbindsIconHoverPath%
    URLDownloadToFile, %groupbindsIconHoverURL%, %groupbindsIconHoverPath%
IfNotExist, %overlaysIconHoverPath%
    URLDownloadToFile, %overlaysIconHoverURL%, %overlaysIconHoverPath%
IfNotExist, %settingsIconHoverPath%
    URLDownloadToFile, %settingsIconHoverURL%, %settingsIconHoverPath%

;================================================================
; Global Main GUI Vars
;================================================================
; Dummy Vars
global HomeSection
global OverlaysSection
global SettingsSection

; Sidebar Menu Vars
global ActiveTab := "Home"
global TooltipTexts := ["Home", "Keybinds", "Textbinds", "Posbinds", "Frakbinds", "Groupbinds", "Overlays", "Settings"]
global Home, Keybinds, Textbinds, Posbinds, Frakbinds, Groupbinds, Overlays, Settings

; Control Buttons
global CloseAuth, LoginButton               ; Image Buttons on Auth GUI
global Signout, MinimizeMain, CloseMain     ; Image Buttons on Main GUI

; Menu: Keybinds
global KeybindsSave, KeybindsGroupbox
global KeybindTrigger1, KeybindTrigger2, KeybindTrigger3, KeybindTrigger4, KeybindTrigger5, KeybindTrigger6, KeybindTrigger7, KeybindTrigger8, KeybindTrigger9, KeybindTrigger10, KeybindTrigger11, KeybindTrigger12, KeybindTrigger13, KeybindTrigger14, KeybindTrigger15, KeybindTrigger16, KeybindTrigger17, KeybindTrigger18, KeybindTrigger19, KeybindTrigger20
global KeybindAction1, KeybindAction2, KeybindAction3, KeybindAction4, KeybindAction5, KeybindAction6, KeybindAction7, KeybindAction8, KeybindAction9, KeybindAction10, KeybindAction11, KeybindAction12, KeybindAction13, KeybindAction14, KeybindAction15, KeybindAction16, KeybindAction17, KeybindAction18, KeybindAction19, KeybindAction20

; Menu: Textbinds
global TextbindsSave, TextbindsGroupbox
global TextbindTrigger1, TextbindTrigger2, TextbindTrigger3, TextbindTrigger4, TextbindTrigger5, TextbindTrigger6, TextbindTrigger7, TextbindTrigger8, TextbindTrigger9, TextbindTrigger10, TextbindTrigger11, TextbindTrigger12, TextbindTrigger13, TextbindTrigger14, TextbindTrigger15, TextbindTrigger16, TextbindTrigger17, TextbindTrigger18, TextbindTrigger19, TextbindTrigger20
global TextbindAction1, TextbindAction2, TextbindAction3, TextbindAction4, TextbindAction5, TextbindAction6, TextbindAction7, TextbindAction8, TextbindAction9, TextbindAction10, TextbindAction11, TextbindAction12, TextbindAction13, TextbindAction14, TextbindAction15, TextbindAction16, TextbindAction17, TextbindAction18, TextbindAction19, TextbindAction20

; Menu: Posbinds
global PosbindsSave, PosbindsGroupbox
global PosbindTrigger1, PosbindTrigger2, PosbindTrigger3, PosbindTrigger4, PosbindTrigger5, PosbindTrigger6, PosbindTrigger7, PosbindTrigger8, PosbindTrigger9, PosbindTrigger10, PosbindTrigger11, PosbindTrigger12, PosbindTrigger13, PosbindTrigger14, PosbindTrigger15, PosbindTrigger16, PosbindTrigger17, PosbindTrigger18, PosbindTrigger19, PosbindTrigger20
global PosbindAction1, PosbindAction2, PosbindAction3, PosbindAction4, PosbindAction5, PosbindAction6, PosbindAction7, PosbindAction8, PosbindAction9, PosbindAction10, PosbindAction11, PosbindAction12, PosbindAction13, PosbindAction14, PosbindAction15, PosbindAction16, PosbindAction17, PosbindAction18, PosbindAction19, PosbindAction20

; Menu: Frakbinds
global FrakbindsSave, FrakbindsGroupbox
global FrakbindTrigger1, FrakbindTrigger2, FrakbindTrigger3, FrakbindTrigger4, FrakbindTrigger5, FrakbindTrigger6, FrakbindTrigger7, FrakbindTrigger8, FrakbindTrigger9, FrakbindTrigger10, FrakbindTrigger11, FrakbindTrigger12, FrakbindTrigger13, FrakbindTrigger14, FrakbindTrigger15, FrakbindTrigger16, FrakbindTrigger17, FrakbindTrigger18, FrakbindTrigger19, FrakbindTrigger20
global FrakbindAction1, FrakbindAction2, FrakbindAction3, FrakbindAction4, FrakbindAction5, FrakbindAction6, FrakbindAction7, FrakbindAction8, FrakbindAction9, FrakbindAction10, FrakbindAction11, FrakbindAction12, FrakbindAction13, FrakbindAction14, FrakbindAction15, FrakbindAction16, FrakbindAction17, FrakbindAction18, FrakbindAction19, FrakbindAction20

; Menu: Groupbinds
global GroupbindsSave, GroupbindsGroupbox
global GroupbindTrigger1, GroupbindTrigger2, GroupbindTrigger3, GroupbindTrigger4, GroupbindTrigger5, GroupbindTrigger6, GroupbindTrigger7, GroupbindTrigger8, GroupbindTrigger9, GroupbindTrigger10, GroupbindTrigger11, GroupbindTrigger12, GroupbindTrigger13, GroupbindTrigger14, GroupbindTrigger15, GroupbindTrigger16, GroupbindTrigger17, GroupbindTrigger18, GroupbindTrigger19, GroupbindTrigger20
global GroupbindAction1, GroupbindAction2, GroupbindAction3, GroupbindAction4, GroupbindAction5, GroupbindAction6, GroupbindAction7, GroupbindAction8, GroupbindAction9, GroupbindAction10, GroupbindAction11, GroupbindAction12, GroupbindAction13, GroupbindAction14, GroupbindAction15, GroupbindAction16, GroupbindAction17, GroupbindAction18, GroupbindAction19, GroupbindAction20

;================================================================
; Authentication Logic
;================================================================
OutputDebug, [Socket.IO] -> Connecting...
Socket.Connect("http://127.0.0.1:4325")

if (Socket.connected) {
    OutputDebug, [Socket.IO] <- Connected
    Socket.On("message", "HandleServerMessage")
    Socket.On("loginResponse", "HandleLoginResponse")
}

credentials := LoadCredentials()
if (credentials) {
    Username := credentials.username
    Password := credentials.password
    GoSub, LoginButton
}
else {
    ShowLoginGUI()
}
OutputDebug, [GUI] -> Login GUI Loaded
return

;================================================================
; Credential Storage
;================================================================
SaveCredentials(username, password) {
    FileDelete, %CredentialsFile%
    IniWrite, %username%, %CredentialsFile%, Credentials, Username
    IniWrite, %password%, %CredentialsFile%, Credentials, Password
}

LoadCredentials() {
    if (FileExist(CredentialsFile)) {
        IniRead, savedUsername, %CredentialsFile%, Credentials, Username
        IniRead, savedPassword, %CredentialsFile%, Credentials, Password
        if (savedUsername != "ERROR" && savedPassword != "ERROR") {
            return {username: savedUsername, password: savedPassword}
        }
    }
    return false
}

ClearCredentials() {
    FileDelete, %CredentialsFile%
}

;================================================================
; Button Handler
;================================================================
LoginButton:
    OutputDebug, [Socket.IO] -> AUTH
    Gui, Login:Submit, NoHide
    ; Prepare the data for authentication
    authData := {}
    authData.username := Username
    authData.password := Password

    ; Send authentication request via SocketIO
    Socket.Emit("auth", JSON.Dump(authData))
return

CloseApp:
    ExitApp
return

MinimizeApp:
    Gui, Main:Minimize
return

LogoutApp:
    FileDelete, %CredentialsFile%
    Gui, Main:Destroy
    ShowLoginGUI()
return

;================================================================
; SIO Event Handler
;================================================================
HandleServerMessage(data) {
    OutputDebug, Message Received: %data%
}

HandleLoginResponse(data) {
    response := JSON.Load(data)
    message := response.message
    if (response.success) {
        OutputDebug, [Socket.IO] <- AUTH OK
        GuiControlGet, RememberLogin
        SaveCredentials(Username, Password)
        Gui, Login:Destroy        
        ShowMainGUI()
        OutputDebug, [GUI] -> Main GUI Loaded
    } else {
        OutputDebug, [Socket.IO] <- AUTH FAILED
        MsgBox, Die Anmeldung ist fehlgeschlagen.
    }
}

;================================================================
; GUI Event Handler
;================================================================
SwitchTab:
    GuiControlGet, clickedControl, Name, % A_GuiControl
    
    ; Reset all icons to default
    GuiControl, Main:, Home, %homeIconPath%
    GuiControl, Main:, Keybinds, %keybindsIconPath%
    GuiControl, Main:, Textbinds, %textbindsIconPath%
    GuiControl, Main:, Posbinds, %posbindsIconPath%
    GuiControl, Main:, Frakbinds, %frakbindsIconPath%
    GuiControl, Main:, Groupbinds, %groupbindsIconPath%
    GuiControl, Main:, Overlays, %overlaysIconPath%
    GuiControl, Main:, Settings, %settingsIconPath%
    
    ; Hide all sections
    GuiControl, Hide, HomeSection

    ; Hide Keybind Elements
    GuiControl, Hide, KeybindsGroupbox
    Loop, 20 {
        GuiControl, Hide, KeybindTrigger%A_Index%
        GuiControl, Hide, KeybindAction%A_Index%
    }
    GuiControl, Hide, KeybindsSave

    ; Hide Textbind Elements
    GuiControl, Hide, TextbindsGroupbox
    Loop, 20 {
        GuiControl, Hide, TextbindTrigger%A_Index%
        GuiControl, Hide, TextbindAction%A_Index%
    }
    GuiControl, Hide, TextbindsSave

    ; Hide Posbind Elements
    GuiControl, Hide, PosbindsGroupbox
    Loop, 20 {
        GuiControl, Hide, PosbindTrigger%A_Index%
        GuiControl, Hide, PosbindAction%A_Index%
    }
    GuiControl, Hide, PosbindsSave

    ; Hide Frakbind Elements
    GuiControl, Hide, FrakbindsGroupbox
    Loop, 20 {
        GuiControl, Hide, FrakbindTrigger%A_Index%
        GuiControl, Hide, FrakbindAction%A_Index%
    }
    GuiControl, Hide, FrakbindsSave

    ; Hide Groupbind Elements
    GuiControl, Hide, GroupbindsGroupbox
    Loop, 20 {
        GuiControl, Hide, GroupbindTrigger%A_Index%
        GuiControl, Hide, GroupbindAction%A_Index%
    }
    GuiControl, Hide, GroupbindsSave


    GuiControl, Hide, OverlaysSection
    GuiControl, Hide, SettingsSection
    
    ; Switch section and set hover state based on clicked control
    OutputDebug, [GUI] -> Switchting to %clickedControl%
    Switch clickedControl {
        Case "Home":
            GuiControl, Main:, Home, %homeIconHoverPath%
            GuiControl, Show, HomeSection
            ActiveTab := "Home"
        Case "Keybinds":
            GuiControl, Main:, Keybinds, %keybindsIconHoverPath%
            GuiControl, Show, KeybindsGroupbox
            Loop, 20 {
                GuiControl, Show, KeybindTrigger%A_Index%
                GuiControl, Show, KeybindAction%A_Index%
            }
            GuiControl, Show, KeybindsSave
            ActiveTab := "Keybinds"
        Case "Textbinds":
            GuiControl, Main:, Textbinds, %textbindsIconHoverPath%
            GuiControl, Show, TextbindsGroupbox   
            Loop, 20 {
                GuiControl, Show, TextbindTrigger%A_Index%
                GuiControl, Show, TextbindAction%A_Index%
            }         
            GuiControl, Show, TextbindsSave
            ActiveTab := "Textbinds"
        Case "Posbinds":
            GuiControl, Main:, Posbinds, %posbindsIconHoverPath%
            GuiControl, Show, PosbindsGroupbox   
            Loop, 20 {
                GuiControl, Show, PosbindTrigger%A_Index%
                GuiControl, Show, PosbindAction%A_Index%
            }         
            GuiControl, Show, PosbindsSave
            ActiveTab := "Posbinds"
        Case "Frakbinds":
            GuiControl, Main:, Frakbinds, %frakbindsIconHoverPath%
            GuiControl, Show, FrakbindsGroupbox   
            Loop, 20 {
                GuiControl, Show, FrakbindTrigger%A_Index%
                GuiControl, Show, FrakbindAction%A_Index%
            }         
            GuiControl, Show, FrakbindsSave
            ActiveTab := "Frakbinds"
        Case "Groupbinds":
            GuiControl, Main:, Groupbinds, %GroupbindsIconHoverPath%
            GuiControl, Show, GroupbindsGroupbox   
            Loop, 20 {
                GuiControl, Show, GroupbindTrigger%A_Index%
                GuiControl, Show, GroupbindAction%A_Index%
            }         
            GuiControl, Show, GroupbindsSave
            ActiveTab := "Groupbinds"
        Case "Overlays":
            GuiControl, Main:, Overlays, %overlaysIconHoverPath%
            GuiControl, Show, OverlaysSection
            ActiveTab := "Overlays"
        Case "Settings":
            GuiControl, Main:, Settings, %settingsIconHoverPath%
            GuiControl, Show, SettingsSection
            ActiveTab := "Settings"
    }
return

Nova:
    Run, https://nova-network.one/
return

; WM Events
WM_MOUSEMOVE(wParam, lParam, msg, hwnd) {
    static hoveredControl := ""
    
    if (wParam = 1) {
        PostMessage, 0xA1, 2,,, A
        return
    }
    
    MouseGetPos,,, win, control
    WinGetTitle, activeTitle, ahk_id %win%
    GuiControlGet, controlName, Name, %control%
    
    if (controlName != hoveredControl) {
        ; Reset previous hover states
        if (InStr(activeTitle, "Authentifizierung")) {
            if (InStr(hoveredControl, "CloseAuth")) {
                GuiControl, Login:, CloseAuth, %closeIconPath%
            } 
            else if (InStr(hoveredControl, "LoginButton")) {
                GuiControl, Login:, LoginButton, %loginIconPath%
            }
            
            ; Set new hover states
            if (InStr(controlName, "CloseAuth")) {
                GuiControl, Login:, CloseAuth, %closeIconHoverPath%
            } 
            else if (InStr(controlName, "LoginButton")) {
                GuiControl, Login:, LoginButton, %loginIconHoverPath%
            }
        } else {
            if (HasVal(TooltipTexts, controlName)) {
                ToolTip, % controlName, mouseX + 15, mouseY + 15
            } else {
                ToolTip
            }

            if (hoveredControl) {
                Switch hoveredControl {
                    Case "CloseMain":
                            GuiControl, Main:, CloseMain, %closeIconPath%
                    Case "MinimizeMain":
                            GuiControl, Main:, MinimizeMain, %minimizeIconPath%
                    Case "Signout":
                            GuiControl, Main:, Signout, %logoutIconPath%
                    Case "Home":
                        if (ActiveTab != "Home")
                            GuiControl, Main:, Home, %homeIconPath%
                    Case "Keybinds":
                        if (ActiveTab != "Keybinds")
                            GuiControl, Main:, Keybinds, %keybindsIconPath%
                    Case "Textbinds":
                        if (ActiveTab != "Textbinds")
                            GuiControl, Main:, Textbinds, %textbindsIconPath%
                    Case "Posbinds":
                        if (ActiveTab != "Posbinds")
                            GuiControl, Main:, Posbinds, %posbindsIconPath%
                    Case "Frakbinds":
                        if (ActiveTab != "Frakbinds")
                            GuiControl, Main:, Frakbinds, %frakbindsIconPath%
                    Case "Groupbinds":
                        if (ActiveTab != "Groupbinds")
                            GuiControl, Main:, Groupbinds, %groupbindsIconPath%
                    Case "Overlays":
                        if (ActiveTab != "Overlays")
                            GuiControl, Main:, Overlays, %overlaysIconPath%
                    Case "Settings":
                        if (ActiveTab != "Settings")
                            GuiControl, Main:, Settings, %settingsIconPath%
                }
            }
            
            Switch controlName {
                Case "CloseMain":
                    GuiControl, Main:, CloseMain, %closeIconHoverPath%
                Case "MinimizeMain":
                    GuiControl, Main:, MinimizeMain, %minimizeIconHoverPath%
                Case "Signout":
                    GuiControl, Main:, Signout, %logoutIconHoverPath%
                Case "Home":
                    if (ActiveTab != "Home")
                        GuiControl, Main:, Home, %homeIconHoverPath%
                Case "Keybinds":
                    if (ActiveTab != "Keybinds")
                        GuiControl, Main:, Keybinds, %keybindsIconHoverPath%
                Case "Textbinds":
                    if (ActiveTab != "Textbinds")
                        GuiControl, Main:, Textbinds, %textbindsIconHoverPath%
                Case "Posbinds":
                    if (ActiveTab != "Posbinds")
                        GuiControl, Main:, Posbinds, %posbindsIconHoverPath%
                Case "Frakbinds":
                    if (ActiveTab != "Frakbinds")
                        GuiControl, Main:, Frakbinds, %frakbindsIconHoverPath%
                Case "Groupbinds":
                    if (ActiveTab != "Groupbinds")
                        GuiControl, Main:, Groupbinds, %groupbindsIconHoverPath%
                Case "Overlays":
                    if (ActiveTab != "Overlays")
                        GuiControl, Main:, Overlays, %overlaysIconHoverPath%
                Case "Settings":
                    if (ActiveTab != "Settings")
                        GuiControl, Main:, Settings, %settingsIconHoverPath%
            }
        }
        hoveredControl := controlName
    }
}

WM_KEYDOWN(wParam, lParam) {
    if (wParam = 0x0D)
    {
        ControlGetFocus, focusedControl, A
        if (focusedControl = "Edit1" || focusedControl = "Edit2")
        {
            Gosub, LoginButton
        }
    }
    return
}

HasVal(haystack, needle) {
	if !(IsObject(haystack)) || (haystack.Length() = 0)
		return 0
	for index, value in haystack
		if (value = needle)
			return index
	return 0
}

;================================================================
; GUIs
;================================================================
ShowLoginGUI() {
    Gui, Login:New, -Resize -Caption +ToolWindow +0x400000
    Gui, Color, 0x1E1E1E

    ; Add a custom title bar
    Gui, Font, s12 Bold, Segoe UI
    Gui, Login:Add, Text, x56 y10 w300 h30 cFFFFFF Center, aBinder Authentifizierung
    Gui, Font, s10 Normal, Segoe UI
    Gui, Login:Add, Text, x56 y40 w300 h20 cFFFFFF Center, build for Nova Network
    Gui, Login:Add, Picture, gCloseApp vCloseAuth x372 y10 w20 h20, %closeIconPath%
    
    ; Add username and password fields
    Gui, Login:Add, Text, x10 y70 w300 h20 cFFFFFF, Benutzername
    Gui, Login:Add, Edit, vUsername x10 y90 w380 h30 c000000 Background0x2A2A2A
    Gui, Login:Add, Text, x10 y130 w300 h20 cFFFFFF, Passwort
    Gui, Login:Add, Edit, vPassword x10 y150 w380 h30 Password c000000 Background0x2A2A2A

    ; Add login button
    Gui, Login:Add, Picture, gLoginButton vLoginButton x350 y190 w40 h40, %loginIconPath%

    ; Add build version text
    Gui, Login:Add, Text, x10 y215 w300 h20 cFFFFFF, aBinder Build %version%

    ; Show the GUI
    Gui, Login:Show, w400 h235, aBinder Authentifizierung
    
    ; Movable GUI
    OnMessage( 0x200, "WM_MOUSEMOVE" ) 

    ; Set Enter key handler
    Gui, Login:Default
    GuiControl, +WantReturn, Username
    GuiControl, +WantReturn, Password
    OnMessage(0x100, "WM_KEYDOWN")
}

ShowMainGUI() {
    Gui, Main:New, -Resize -Caption +ToolWindow +0x400000
    Gui, Color, 0x1E1E1E

    ; Add a custom title bar
    Gui, Font, s12 Bold, Segoe UI
    Gui, Main:Add, Text, x70 y10 w400 h30 cFFFFFF, aBinder - Advanced Binding Made Simple.
    Gui, Font, s10 Normal, Segoe UI
    Gui, Main:Add, Picture, gLogoutApp vSignout x940 y10 w20 h20, %logoutIconPath%
    Gui, Main:Add, Picture, gMinimizeApp vMinimizeMain x970 y10 w20 h20, %minimizeIconPath%
    Gui, Main:Add, Picture, gCloseApp vCloseMain x1000 y10 w20 h20, %closeIconPath%

    ; Add sidebar menu
    Gui, Main:Add, Picture, gNova x10 y10 w40 h40, %novaIconPath%
    Gui, Main:Add, Picture, gSwitchTab vHome x10 y60 w40 h40, %homeIconHoverPath%
    Gui, Main:Add, Picture, gSwitchTab vKeybinds x10 y110 w40 h40, %keybindsIconPath%
    Gui, Main:Add, Picture, gSwitchTab vTextbinds x10 y160 w40 h40, %textbindsIconPath%
    Gui, Main:Add, Picture, gSwitchTab vPosbinds x10 y210 w40 h40, %posbindsIconPath%
    Gui, Main:Add, Picture, gSwitchTab vFrakbinds x10 y260 w40 h40, %frakbindsIconPath%
    Gui, Main:Add, Picture, gSwitchTab vGroupbinds x10 y310 w40 h40, %groupbindsIconPath%
    Gui, Main:Add, Picture, gSwitchTab vOverlays x10 y360 w40 h40, %overlaysIconPath%
    Gui, Main:Add, Picture, gSwitchTab vSettings x10 y410 w40 h40, %settingsIconPath%

    ; Home Section
    Gui, Main:Add, Text, vHomeSection x70 y50 w930 h500 cFFFFFF, Home Menu Content

    ; Keybinds Section
    Gui, Main:Add, GroupBox, vKeybindsGroupbox x60 y60 w960 h480 cFFFFFF Hidden, Keybinds
    counter := 1
    Loop, 10 {
        y := 90 + (A_Index - 1) * 40
        Gui, Main:Add, Hotkey, vKeybindTrigger%counter% x70 y%y% w150 h25 Hidden
        Gui, Main:Add, Edit, vKeybindAction%counter% x230 y%y% w300 h25 Hidden
        counter++
    }
    Loop, 10 {
        y := 90 + (A_Index - 1) * 40
        Gui, Main:Add, Hotkey, vKeybindTrigger%counter% x550 y%y% w150 h25 Hidden
        Gui, Main:Add, Edit, vKeybindAction%counter% x710 y%y% w300 h25 Hidden
        counter++
    }
    Gui, Main:Add, Button, vKeybindsSave x920 y510 w90 h25 Hidden, Speichern

    ; Textbinds Section
    Gui, Main:Add, GroupBox, vTextbindsGroupbox x60 y60 w960 h480 cFFFFFF Hidden, Textbinds
    counter := 1
    Loop, 10 {
        y := 90 + (A_Index - 1) * 40
        Gui, Main:Add, Edit, vTextbindTrigger%counter% x70 y%y% w150 h25 Hidden
        Gui, Main:Add, Edit, vTextbindAction%counter% x230 y%y% w300 h25 Hidden
        counter++
    }
    Loop, 10 {
        y := 90 + (A_Index - 1) * 40
        Gui, Main:Add, Edit, vTextbindTrigger%counter% x550 y%y% w150 h25 Hidden
        Gui, Main:Add, Edit, vTextbindAction%counter% x710 y%y% w300 h25 Hidden
        counter++
    }
    Gui, Main:Add, Button, vTextbindsSave x920 y510 w90 h25 Hidden, Speichern

    ; Posbinds Section
    Gui, Main:Add, GroupBox, vPosbindsGroupbox x60 y60 w960 h480 cFFFFFF Hidden, Posbinds
    counter := 1
    Loop, 10 {
        y := 90 + (A_Index - 1) * 40
        Gui, Main:Add, Edit, vPosbindTrigger%counter% x70 y%y% w150 h25 Hidden
        Gui, Main:Add, Edit, vPosbindAction%counter% x230 y%y% w300 h25 Hidden
        counter++
    }
    Loop, 10 {
        y := 90 + (A_Index - 1) * 40
        Gui, Main:Add, Edit, vPosbindTrigger%counter% x550 y%y% w150 h25 Hidden
        Gui, Main:Add, Edit, vPosbindAction%counter% x710 y%y% w300 h25 Hidden
        counter++
    }
    Gui, Main:Add, Button, vPosbindsSave x920 y510 w90 h25 Hidden, Speichern

    ; Frakbinds Section
    Gui, Main:Add, GroupBox, vFrakbindsGroupbox x60 y60 w960 h480 cFFFFFF Hidden, Frakbinds
    counter := 1
    Loop, 10 {
        y := 90 + (A_Index - 1) * 40
        Gui, Main:Add, Hotkey, vFrakbindTrigger%counter% x70 y%y% w150 h25 Hidden
        Gui, Main:Add, Edit, vFrakbindAction%counter% x230 y%y% w300 h25 Hidden
        counter++
    }
    Loop, 10 {
        y := 90 + (A_Index - 1) * 40
        Gui, Main:Add, Hotkey, vFrakbindTrigger%counter% x550 y%y% w150 h25 Hidden
        Gui, Main:Add, Edit, vFrakbindAction%counter% x710 y%y% w300 h25 Hidden
        counter++
    }
    Gui, Main:Add, Button, vFrakbindsSave x920 y510 w90 h25 Hidden, Speichern

    ; Groupbinds Section
    Gui, Main:Add, GroupBox, vGroupbindsGroupbox x60 y60 w960 h480 cFFFFFF Hidden, Groupbinds
    counter := 1
    Loop, 10 {
        y := 90 + (A_Index - 1) * 40
        Gui, Main:Add, Hotkey, vGroupbindTrigger%counter% x70 y%y% w150 h25 Hidden
        Gui, Main:Add, Edit, vGroupbindAction%counter% x230 y%y% w300 h25 Hidden
        counter++
    }
    Loop, 10 {
        y := 90 + (A_Index - 1) * 40
        Gui, Main:Add, Hotkey, vGroupbindTrigger%counter% x550 y%y% w150 h25 Hidden
        Gui, Main:Add, Edit, vGroupbindAction%counter% x710 y%y% w300 h25 Hidden
        counter++
    }
    Gui, Main:Add, Button, vGroupbindsSave x920 y510 w90 h25 Hidden, Speichern

    ; Overlays Section
    Gui, Main:Add, Text, vOverlaysSection x70 y50 w930 h500 cFFFFFF Hidden, Overlays Menu Content

    ; Settings Section
    Gui, Main:Add, Text, vSettingsSection x70 y50 w930 h500 cFFFFFF Hidden, Settings Menu Content

    ; Show the GUI
    Gui, Main:Show, w1030 h550, aBinder
    
    ; Movable GUI
    OnMessage( 0x200, "WM_MOUSEMOVE" )
}