import { Server, Socket } from 'socket.io';

export const setupPositionHandlers = (io: Server, socket: Socket) => {
    // Platzhalter für Positions-Handler
    socket.on('position:update', (data: any) => {
        // Hier später die Positions-Logik implementieren
        console.log('Position Update empfangen:', data);
    });
}; 