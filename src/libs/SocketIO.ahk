class SocketIO {
    __New() {
        static wb
        
        Gui, +hWndhOld
        Gui, New, +hWndhWnd
        
        this.hWnd := hWnd
        this.connected := false
        
        Gui, Add, ActiveX, vWB, Shell.Explorer
        Gui, %hOld%: Default
        
        WB.Navigate("about:<!DOCTYPE html><meta http-equiv='X-UA-Compatible' content='IE=edge'><head><script src='https://cdnjs.cloudflare.com/ajax/libs/socket.io/4.8.1/socket.io.js'></script></head><body></body>")
        
        while (WB.ReadyState < 4)
            sleep, 50
        
        this.document := WB.document
        this.document.parentWindow.ahk_savews := this._SaveWS.Bind(this)
        this.document.parentWindow.ahk_event := this._Event.Bind(this)
        
        Script := this.document.createElement("script")
        Script.text := "var sock = null;`n"
                    . "function sockConnect(){ if(sock) {sock.close(); sock = null;} sock = io.connect('http://127.0.0.1:4325', {reconnect: true, secure: true});"
                    . "sock.on('message', function(data){ ahk_event('Message', data); });"
                    . "sock.on('loginResponse', function(data){ ahk_event('LoginResponse', data); });"
                    . "sock.on('disconnect', function(){ ahk_event('Disconnect', 'disconnected'); });"
                    . "sock.on('connect', function(){ ahk_event('Connect', 'connected'); });"
                    . "sock.on('connect_error', function(){ ahk_event('ConnectError', 'error'); });}"
                    . "function sockEmit(type, content){ if(sock) sock.emit(type, content); }`n"
        this.document.body.appendChild(Script)
    }
    
    Connect() {
        try {
            this.document.parentWindow.sockConnect()
        } catch e {
            OutputDebug, [Socket.IO] Connect Error: %e%
            return false
        }
    }
    
    _Event(EventName, Event) {
        this["On" EventName](Event)
    }
    
    Emit(Type, Content) {
        this.document.parentWindow.sockEmit(Type, Content)
    }
    
    Close() {
        this.document.parentWindow.sock.close()
        this.connected := false
    }
    
    Disconnect() {
        if this.hWnd {
            this.Close()
            this.wb.quit
            this.hWnd := False
        }
    }
}

class socketClass extends SocketIO {
    OnConnectError(data) {
        OutputDebug, [Socket.IO] <- CON ERROR 
        MsgBox, 16, Verbindungsfehler, Es konnte keine Verbindung zum Server hergestellt werden. Überprüfe deine Internetverbindung oder probiere es später erneut.
        ExitApp
    }

    OnConnect() {
        OutputDebug, [Socket.IO] <- Pong 
        this.connected := true
    }

    OnConnected(data) {
        OutputDebug, OnConnected Event
    }

    OnMessage(data) {
        OutputDebug, [Socket.IO] <- MSG
        HandleServerMessage(data)
    }

    OnLoginResponse(data) {
        HandleLoginResponse(data)
    }
}