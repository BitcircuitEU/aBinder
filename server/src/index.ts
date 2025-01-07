import dotenv from 'dotenv';
import { app, httpsServer, io } from './config/server';
import connectDB from './config/database';
import authRoutes from './routes/auth';
import { authenticateSocket } from './middleware/auth';
import { registerChatHandlers } from './socket/chatHandlers';
import { registerBindHandlers } from './socket/bindHandlers';
import { registerPositionHandlers } from './socket/positionHandlers';
import { PositionService } from './services/positionService';

dotenv.config();

// Verbindung zur Datenbank herstellen
connectDB();

// Express Routes
app.use('/api/auth', authRoutes);

// Socket.IO Middleware
io.use(authenticateSocket);

// Socket.IO Event Handlers
io.on('connection', (socket: any) => {
  console.log(`Client verbunden: ${socket.user.username}`);

  // Räume für Nutzer beitreten
  socket.join(`user:${socket.user.id}`);
  
  if (socket.user.groupId !== '0') {
    socket.join(`group:${socket.user.groupId}`);
  }
  
  if (socket.user.factionId !== '0') {
    socket.join(`faction:${socket.user.factionId}`);
  }

  // Handler registrieren
  registerChatHandlers(socket);
  registerBindHandlers(socket);
  registerPositionHandlers(socket);

  socket.on('disconnect', () => {
    console.log(`Client getrennt: ${socket.user.username}`);
  });
});

// Express-Route für Gesundheitscheck
app.get('/health', (req, res) => {
  res.json({ status: 'OK' });
});

// Regelmäßige Bereinigung alter Positionen (alle 5 Minuten)
setInterval(() => {
  PositionService.cleanupOldPositions();
}, 5 * 60 * 1000);

// Server starten
const PORT = 4325;
httpsServer.listen(PORT, () => {
  console.log(`Server läuft auf Port ${PORT}`);
}); 