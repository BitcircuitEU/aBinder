class SocketIO {
    __New() {
        static wb
        
        Gui, +hWndhOld
        Gui, New, +hWndhWnd
        
        this.hWnd := hWnd
        
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
                    . "ahk_event('Connect', 'connected');}"
                    . "function sockEmit(type, content){ if(sock) sock.emit(type, content); }`n"
        this.document.body.appendChild(Script)
        this.Connect()
    }
    
    Connect() {
        this.document.parentWindow.sockConnect()
    }
    
    _Event(EventName, Event) {
        this["On" EventName](Event)
    }
    
    Emit(Type, Content) {
        this.document.parentWindow.sockEmit(Type, Content)
    }
    
    Close() {
        this.document.parentWindow.sock.close()
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
    OnConnect() {
        OutputDebug, [Socket.IO] <- Pong 
    }
    
    OnConnectError(data) {
        OutputDebug, [Socket.IO] <- CON ERROR 
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