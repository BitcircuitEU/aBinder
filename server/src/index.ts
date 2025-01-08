import dotenv from 'dotenv';
import { connectDB } from './config/database';
import { app, server, io } from './config/server';
import authRoutes from './routes/auth';
import { setupSocketHandlers } from './socket/chatHandlers';
import { setupBindHandlers } from './socket/bindHandlers';
import { setupPositionHandlers } from './socket/positionHandlers';

// Deklariere globale Token-Map
declare global {
    var validTokens: Map<string, string>;
}
global.validTokens = new Map();

// Lade Umgebungsvariablen und verbinde zur Datenbank
dotenv.config();
connectDB();

// Routes
app.use('/api/auth', authRoutes);

// Socket.IO Event Handler für neue Verbindungen
io.on('connection', (socket) => {
    console.log('\n=== Client erfolgreich authentifiziert ===');
    console.log('Socket ID:', socket.id);
    console.log('Benutzer:', socket.data.username);
    console.log('IP:', socket.handshake.address);
    console.log('Transport:', socket.conn.transport.name);
    console.log('Protocol:', socket.conn.protocol);
    
    // Sende initiale Benutzerliste
    const users = Array.from(io.sockets.sockets.values())
        .map(s => s.data.username)
        .filter(Boolean);
    
    console.log('Aktive Benutzer:', users.length);
    console.log('Benutzerliste:', users.join(', '));
    
    // Sende Benutzerliste an den neuen Client
    socket.emit('chat:userList', { users });

    // Informiere andere über den neuen Benutzer
    socket.broadcast.emit('chat:userStatus', {
        username: socket.data.username,
        online: true
    });

    // Trete automatisch dem Global-Channel bei
    socket.join('global');
    console.log('Benutzer', socket.data.username, 'ist dem Global-Channel beigetreten');
    socket.emit('chat:joined', { channel: 'global' });

    // Setup Event Handler
    setupSocketHandlers(io, socket);
    setupBindHandlers(io, socket);
    setupPositionHandlers(io, socket);
    
    // Debug Events
    socket.conn.on('packet', (packet) => {
        console.log('Socket.IO Packet:', packet.type, packet.data || '');
    });
    
    socket.on('disconnect', (reason) => {
        console.log('\n=== Client getrennt ===');
        console.log('Socket ID:', socket.id);
        console.log('Benutzer:', socket.data.username);
        console.log('Grund:', reason);
        console.log('Transport:', socket.conn.transport?.name);
        
        if (socket.data.username) {
            socket.broadcast.emit('chat:userStatus', {
                username: socket.data.username,
                online: false
            });
        }
    });

    // Ping/Pong Debug
    socket.on('ping', () => {
        console.log('Ping von Client:', socket.id, '(', socket.data.username, ')');
    });
    
    socket.on('pong', () => {
        console.log('Pong von Client:', socket.id, '(', socket.data.username, ')');
    });
}); 