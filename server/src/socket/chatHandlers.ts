import { Socket } from 'socket.io';
import { ChatService } from '../services/chatService';

export const registerChatHandlers = (socket: Socket & { user?: any }) => {
  // Globaler Chat
  socket.on('chat:global', async (content: string) => {
    try {
      const message = await ChatService.sendGlobalMessage(socket.user.id, content);
      socket.broadcast.emit('chat:message', message);
    } catch (error: any) {
      socket.emit('error', { message: error.message });
    }
  });

  // Privater Chat
  socket.on('chat:private', async ({ to, content }: { to: string; content: string }) => {
    try {
      const message = await ChatService.sendPrivateMessage(socket.user.id, to, content);
      socket.to(`user:${to}`).emit('chat:message', message);
    } catch (error: any) {
      socket.emit('error', { message: error.message });
    }
  });

  // Fraktions-Chat
  socket.on('chat:faction', async (content: string) => {
    try {
      if (socket.user.factionId === '0') {
        throw new Error('Du bist in keiner Fraktion');
      }
      const message = await ChatService.sendFactionMessage(socket.user.id, socket.user.factionId, content);
      socket.to(`faction:${socket.user.factionId}`).emit('chat:message', message);
    } catch (error: any) {
      socket.emit('error', { message: error.message });
    }
  });

  // Gruppen-Chat
  socket.on('chat:group', async (content: string) => {
    try {
      if (socket.user.groupId === '0') {
        throw new Error('Du bist in keiner Gruppe');
      }
      const message = await ChatService.sendGroupMessage(socket.user.id, socket.user.groupId, content);
      socket.to(`group:${socket.user.groupId}`).emit('chat:message', message);
    } catch (error: any) {
      socket.emit('error', { message: error.message });
    }
  });

  // IRC Channel
  socket.on('irc:create', async ({ name, pin }: { name: string; pin: string }) => {
    try {
      const channel = await ChatService.createIRCChannel(name, pin, socket.user.id);
      socket.join(`irc:${channel.id}`);
      socket.emit('irc:joined', channel);
    } catch (error: any) {
      socket.emit('error', { message: error.message });
    }
  });

  socket.on('irc:join', async ({ channelId, pin }: { channelId: string; pin: string }) => {
    try {
      await ChatService.joinIRCChannel(channelId, socket.user.id, pin);
      socket.join(`irc:${channelId}`);
      socket.emit('irc:joined', { channelId });
    } catch (error: any) {
      socket.emit('error', { message: error.message });
    }
  });

  socket.on('irc:message', async ({ channelId, content }: { channelId: string; content: string }) => {
    try {
      const message = await ChatService.sendIRCMessage(socket.user.id, channelId, content);
      socket.to(`irc:${channelId}`).emit('chat:message', message);
    } catch (error: any) {
      socket.emit('error', { message: error.message });
    }
  });

  // IRC Moderation
  socket.on('irc:mute', async ({ channelId, userId }: { channelId: string; userId: string }) => {
    try {
      await ChatService.muteUser(channelId, userId, socket.user.id);
      socket.to(`irc:${channelId}`).emit('irc:userMuted', { userId });
    } catch (error: any) {
      socket.emit('error', { message: error.message });
    }
  });

  socket.on('irc:unmute', async ({ channelId, userId }: { channelId: string; userId: string }) => {
    try {
      await ChatService.unmuteUser(channelId, userId, socket.user.id);
      socket.to(`irc:${channelId}`).emit('irc:userUnmuted', { userId });
    } catch (error: any) {
      socket.emit('error', { message: error.message });
    }
  });

  socket.on('irc:ban', async ({ channelId, userId }: { channelId: string; userId: string }) => {
    try {
      await ChatService.banUser(channelId, userId, socket.user.id);
      socket.to(`irc:${channelId}`).emit('irc:userBanned', { userId });
      socket.to(`user:${userId}`).emit('irc:banned', { channelId });
    } catch (error: any) {
      socket.emit('error', { message: error.message });
    }
  });

  socket.on('irc:promote', async ({ channelId, userId }: { channelId: string; userId: string }) => {
    try {
      await ChatService.promoteModerator(channelId, userId, socket.user.id);
      socket.to(`irc:${channelId}`).emit('irc:moderatorPromoted', { userId });
    } catch (error: any) {
      socket.emit('error', { message: error.message });
    }
  });

  // Chat-Historie abrufen
  socket.on('chat:getHistory', async ({ type, target, limit }: { type: string; target?: string; limit?: number }) => {
    try {
      let history;
      switch (type) {
        case 'global':
          history = await ChatService.getGlobalHistory(limit);
          break;
        case 'private':
          if (!target) throw new Error('Ziel-Benutzer nicht angegeben');
          history = await ChatService.getPrivateHistory(socket.user.id, target, limit);
          break;
        case 'irc':
          if (!target) throw new Error('Channel ID nicht angegeben');
          history = await ChatService.getChannelHistory(target, socket.user.id, limit);
          break;
        default:
          throw new Error('Ungültiger Chat-Typ');
      }
      socket.emit('chat:history', { type, history });
    } catch (error: any) {
      socket.emit('error', { message: error.message });
    }
  });
}; 