#SingleInstance, Force
SetWorkingDir, %A_ScriptDir%

; Binder Presettings
global version := "1.0.0"

; Include necessary libraries
#Include %A_ScriptDir%\libs\JSON.ahk
#Include %A_ScriptDir%\libs\SocketIO.ahk

; Global Socket & Auth variables
global Socket := new socketClass()
global CredentialsFile := A_ScriptDir . "\credentials.ini"
global loggedIn := false
global Username
global Password

; Create icons directory if not exists
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

global novaIconPath := iconsDir "\nova_icon.png"

; Icon file paths
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

; Hover Icon file paths
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

; Sidebar
global MainTab
global ActiveTab := "Home"
global TooltipTexts := ["Home", "Keybinds", "Textbinds", "Posbinds", "Frakbinds", "Groupbinds", "Overlays", "Settings"]

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

; Initialize SocketIO connection
InitializeSocket()

; Show login GUI
credentials := LoadCredentials()
if (credentials) {
    Username := credentials.username
    Password := credentials.password
    InitializeSocket()
    GoSub, LoginButton
}
else {
    ShowLoginGUI()
    InitializeSocket()
}
OutputDebug, [GUI] -> Login GUI Loaded
return

; Helper function to initialize SocketIO connection
InitializeSocket() {
    OutputDebug, [Socket.IO] -> Ping
    Socket.Connect("http://127.0.0.1:4325")
    Socket.On("message", "HandleServerMessage")
    Socket.On("loginResponse", "HandleLoginResponse")
}

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

SaveCredentials(username, password) {
    FileDelete, %CredentialsFile%
    IniWrite, %username%, %CredentialsFile%, Credentials, Username
    IniWrite, %password%, %CredentialsFile%, Credentials, Password
}

; Add new function to load credentials
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

; Handle login button click
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

; Handle close button click
CloseApp:
    ExitApp
return

; Handle minimize button click
MinimizeApp:
    Gui, Main:Minimize
return

; Handle messages from the server
HandleServerMessage(data) {
    OutputDebug, Message Received: %data%
}

; Handle login response from the server
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

ClearCredentials() {
    FileDelete, %CredentialsFile%
}

LogoutApp:
    FileDelete, %CredentialsFile%
    Gui, Main:Destroy
    ShowLoginGUI()
return

; Show main GUI after successful login
global CloseAuth
global CloseMain
global MinimizeMain
global LoginButton
global Signout
global Home
global Keybinds
global Textbinds
global Posbinds
global Frakbinds
global Groupbinds
global Overlays
global Settings
global HomeSection
global KeybindsSection
global TextbindsSection
global PosbindsSection
global FrakbindsSection
global GroupbindsSection
global OverlaysSection
global SettingsSection

ShowMainGUI() {
    Gui, Main:New, -Resize -Caption +ToolWindow +0x400000
    Gui, Color, 0x1E1E1E

    ; Add a custom title bar
    Gui, Font, s12 Bold, Segoe UI
    Gui, Main:Add, Text, x70 y10 w400 h30 cFFFFFF, aBinder - Advanced Binding Made Simple.
    Gui, Font, s10 Normal, Segoe UI
    Gui, Main:Add, Picture, gLogoutApp vSignout x712 y10 w20 h20, %logoutIconPath%
    Gui, Main:Add, Picture, gCloseApp vCloseMain x772 y10 w20 h20, %closeIconPath%
    Gui, Main:Add, Picture, gMinimizeApp vMinimizeMain x742 y10 w20 h20, %minimizeIconPath%

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
    Gui, Main:Add, Text, vHomeSection x70 y50 w730 h500 cFFFFFF, Home Menu Content

    ; Keybinds Section
    Gui, Main:Add, Text, vKeybindsSection x70 y50 w730 h500 cFFFFFF Hidden, Keybinds Menu Content

    ; Textbinds Section
    Gui, Main:Add, Text, vTextbindsSection x70 y50 w730 h500 cFFFFFF Hidden, Textbinds Menu Content

    ; Posbinds Section
    Gui, Main:Add, Text, vPosbindsSection x70 y50 w730 h500 cFFFFFF Hidden, Posbinds Menu Content

    ; Frakbinds Section
    Gui, Main:Add, Text, vFrakbindsSection x70 y50 w730 h500 cFFFFFF Hidden, Frakbinds Menu Content

    ; Groupbinds Section
    Gui, Main:Add, Text, vGroupbindsSection x70 y50 w730 h500 cFFFFFF Hidden, Groupbinds Menu Content

    ; Overlays Section
    Gui, Main:Add, Text, vOverlaysSection x70 y50 w730 h500 cFFFFFF Hidden, Overlays Menu Content

    ; Settings Section
    Gui, Main:Add, Text, vSettingsSection x70 y50 w730 h500 cFFFFFF Hidden, Settings Menu Content

    ; Show the GUI
    Gui, Main:Show, w800 h600, aBinder
    
    ; Movable GUI
    OnMessage( 0x200, "WM_MOUSEMOVE" ) 
}

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
    GuiControl, Hide, KeybindsSection
    GuiControl, Hide, TextbindsSection
    GuiControl, Hide, PosbindsSection
    GuiControl, Hide, FrakbindsSection
    GuiControl, Hide, GroupbindsSection
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
            GuiControl, Show, KeybindsSection
            ActiveTab := "Keybinds"
        Case "Textbinds":
            GuiControl, Main:, Textbinds, %textbindsIconHoverPath%
            GuiControl, Show, TextbindsSection
            ActiveTab := "Textbinds"
        Case "Posbinds":
            GuiControl, Main:, Posbinds, %posbindsIconHoverPath%
            GuiControl, Show, PosbindsSection
            ActiveTab := "Posbinds"
        Case "Frakbinds":
            GuiControl, Main:, Frakbinds, %frakbindsIconHoverPath%
            GuiControl, Show, FrakbindsSection
            ActiveTab := "Frakbinds"
        Case "Groupbinds":
            GuiControl, Main:, Groupbinds, %groupbindsIconHoverPath%
            GuiControl, Show, GroupbindsSection
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
                GuiControl,, Static3, %closeIconPath%
            } 
            else if (InStr(hoveredControl, "LoginButton")) {
                GuiControl,, Static6, %loginIconPath%
            }
            
            ; Set new hover states
            if (InStr(controlName, "CloseAuth")) {
                GuiControl,, Static3, %closeIconHoverPath%
            } 
            else if (InStr(controlName, "LoginButton")) {
                GuiControl,, Static6, %loginIconHoverPath%
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