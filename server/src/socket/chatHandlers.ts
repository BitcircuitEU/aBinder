import { Server, Socket } from 'socket.io';
import { ChatService } from '../services/chatService';

interface AuthenticatedSocket extends Socket {
    data: {
        user: any;
        username: string;
    };
}

export const setupSocketHandlers = (io: Server, socket: AuthenticatedSocket) => {
    console.log(`Chat-Handler für Socket ${socket.id} eingerichtet`);

    socket.on('chat:message', async (data: any) => {
        const { channel, content } = data;
        console.log(`Chat: Nachricht empfangen - Channel: ${channel}, Von: ${socket.data.username}, Inhalt: ${content}`);
        
        try {
            if (!channel || !content) {
                throw new Error('Ungültige Nachrichtendaten');
            }

            // Prüfe ob der Benutzer im Channel ist
            if (!socket.rooms.has(channel)) {
                socket.emit('chat:error', { 
                    message: 'Du bist nicht im Channel' 
                });
                return;
            }

            // Speichere die Nachricht in der Datenbank
            const savedMessage = await ChatService.sendGlobalMessage(socket.data.username, content);
            console.log('Chat: Nachricht gespeichert');

            // Sende die Nachricht an alle Clients im Channel
            io.to(channel).emit('chat:message', {
                channel,
                from: socket.data.username,
                content,
                timestamp: savedMessage.timestamp
            });
        } catch (error) {
            console.error('Chat: Fehler beim Speichern/Senden der Nachricht:', error);
            socket.emit('chat:error', { 
                message: 'Nachricht konnte nicht gesendet werden' 
            });
        }
    });

    socket.on('chat:join', async (data: any) => {
        const { channel } = data;
        console.log(`Chat: Benutzer ${socket.data.username} betritt Channel ${channel}`);
        
        try {
            if (!channel) {
                throw new Error('Kein Channel angegeben');
            }

            // Trete dem Channel bei
            await socket.join(channel);
            socket.emit('chat:joined', { channel });

            // Lade die Channel-Historie
            const history = await ChatService.getGlobalHistory(50);
            console.log(`Chat: Sende Historie für ${channel} (${history.length} Nachrichten)`);
            socket.emit('chat:history', { 
                channel, 
                messages: history.map(msg => ({
                    from: msg.from,
                    content: msg.content,
                    timestamp: msg.timestamp
                }))
            });

            // Sende aktuelle Benutzerliste
            const users = Array.from(io.sockets.sockets.values())
                .map(s => (s as any).data.username)
                .filter(Boolean);

            console.log(`Chat: Sende Benutzerliste an ${channel} (${users.length} Benutzer)`);
            io.to(channel).emit('chat:userList', { users });

            // Informiere andere über den neuen Benutzer
            socket.to(channel).emit('chat:userStatus', {
                username: socket.data.username,
                online: true
            });
        } catch (error) {
            console.error('Chat: Fehler beim Betreten des Channels:', error);
            socket.emit('chat:error', { 
                message: 'Channel konnte nicht betreten werden' 
            });
        }
    });

    socket.on('disconnect', () => {
        console.log(`Chat: Benutzer ${socket.data.username} getrennt`);
        if (socket.data.username) {
            // Informiere alle Channels über den Offline-Status
            for (const room of socket.rooms) {
                if (room !== socket.id) {
                    socket.to(room).emit('chat:userStatus', {
                        username: socket.data.username,
                        online: false
                    });
                }
            }
        }
    });
}; 