import { Server, Socket } from 'socket.io';

export const setupBindHandlers = (io: Server, socket: Socket) => {
    // Platzhalter für Bind-Handler
    socket.on('bind:update', (data: any) => {
        // Hier später die Bind-Logik implementieren
        console.log('Bind Update empfangen:', data);
    });
}; 