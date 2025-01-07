import { Socket } from 'socket.io';
import { PositionService } from '../services/positionService';
import type { PositionUpdate } from '../types';

export const registerPositionHandlers = (socket: Socket & { user?: any }) => {
  // Position Update vom Client
  socket.on('position:update', async (update: Omit<PositionUpdate, 'userId' | 'timestamp'>) => {
    try {
      const posUpdate = await PositionService.updatePosition(socket.user.id, {
        ...update,
        timestamp: new Date()
      });

      // Sende Update an Fraktionsmitglieder
      if (socket.user.factionId !== '0') {
        socket.to(`faction:${socket.user.factionId}`).emit('position:member', posUpdate);
      }

      // Sende Update an Gruppenmitglieder
      if (socket.user.groupId !== '0') {
        socket.to(`group:${socket.user.groupId}`).emit('position:member', posUpdate);
      }
    } catch (error: any) {
      socket.emit('error', { message: error.message });
    }
  });

  // Fraktionsmitglieder-Positionen abrufen
  socket.on('position:getFactionMembers', async () => {
    try {
      if (socket.user.factionId === '0') {
        throw new Error('Du bist in keiner Fraktion');
      }

      const positions = await PositionService.getFactionMemberPositions(socket.user.factionId);
      socket.emit('position:factionMembers', positions);
    } catch (error: any) {
      socket.emit('error', { message: error.message });
    }
  });

  // Gruppenmitglieder-Positionen abrufen
  socket.on('position:getGroupMembers', async () => {
    try {
      if (socket.user.groupId === '0') {
        throw new Error('Du bist in keiner Gruppe');
      }

      const positions = await PositionService.getGroupMemberPositions(socket.user.groupId);
      socket.emit('position:groupMembers', positions);
    } catch (error: any) {
      socket.emit('error', { message: error.message });
    }
  });

  // Position eines bestimmten Spielers abrufen
  socket.on('position:getPlayer', async (targetUserId: string) => {
    try {
      const position = await PositionService.getPlayerPosition(targetUserId);
      if (!position) {
        throw new Error('Keine Position für diesen Spieler verfügbar');
      }
      socket.emit('position:player', position);
    } catch (error: any) {
      socket.emit('error', { message: error.message });
    }
  });
}; 