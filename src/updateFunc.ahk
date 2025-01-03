;================================================================
; Update Funktion
;================================================================
#Include %A_ScriptDir%\libs\JSON.ahk

global version := "1.0.0"
global githubRepo := "BitcircuitEU/aBinder"
global updateURL := "https://api.github.com/repos/" githubRepo "/releases/latest"
global downloadPath := A_Temp "\aBinder_update.exe"

try {
    whr := ComObjCreate("WinHttp.WinHttpRequest.5.1")
    whr.Open("GET", updateURL, true)
    whr.Send()
    whr.WaitForResponse()
    
    response := JSON.Load(whr.ResponseText)
    latestVersion := RegExReplace(response.tag_name, "v")
    latestReleaseName := response.name
    changelog := response.body

    OutputDebug, [Update] Current Version: %version% Latest Version: %latestVersion%
    
    if (latestVersion > version) {
        MsgBox, 68, Update verfügbar, Es wurde ein Update gefunden!`n`nRelease Name: %latestReleaseName%`nAktuelle Version: %version%`nNeue Version: %latestVersion%`n`nChangelog:`n%changelog%`n`nJetzt aktualisieren?
        IfMsgBox, Yes
        {
            downloadUrl := response.assets[1].browser_download_url
            UrlDownloadToFile, %downloadUrl%, %downloadPath%
    
            batchPath := A_Temp "\updater.bat"
            FileAppend,
            (
            @echo off
            timeout /t 2 /nobreak
            del "%A_ScriptFullPath%"
            move "%downloadPath%" "%A_ScriptFullPath%"
            start "" "%A_ScriptFullPath%"
            del "%batchPath%"
            ), %batchPath%
            
            Run, %batchPath%,, Hide
            ExitApp
        }
    }
}
catch e {
    OutputDebug, [Update] Error: %e%
}