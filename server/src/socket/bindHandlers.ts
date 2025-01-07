import { Socket } from 'socket.io';
import { BindService } from '../services/bindService';

export const registerBindHandlers = (socket: Socket & { user?: any }) => {
  // Keybinds
  socket.on('keybind:create', async ({ trigger, action }: { trigger: string; action: string }) => {
    try {
      const bind = await BindService.createKeybind(socket.user.id, trigger, action);
      socket.emit('keybind:created', bind);
    } catch (error: any) {
      socket.emit('error', { message: error.message });
    }
  });

  socket.on('keybind:update', async ({ index, trigger, action }: { index: number; trigger: string; action: string }) => {
    try {
      const bind = await BindService.updateKeybind(socket.user.id, index, trigger, action);
      socket.emit('keybind:updated', { index, bind });
    } catch (error: any) {
      socket.emit('error', { message: error.message });
    }
  });

  // Textbinds
  socket.on('textbind:create', async ({ trigger, action }: { trigger: string; action: string }) => {
    try {
      const bind = await BindService.createTextbind(socket.user.id, trigger, action);
      socket.emit('textbind:created', bind);
    } catch (error: any) {
      socket.emit('error', { message: error.message });
    }
  });

  socket.on('textbind:update', async ({ index, trigger, action }: { index: number; trigger: string; action: string }) => {
    try {
      const bind = await BindService.updateTextbind(socket.user.id, index, trigger, action);
      socket.emit('textbind:updated', { index, bind });
    } catch (error: any) {
      socket.emit('error', { message: error.message });
    }
  });

  // Commandbinds
  socket.on('commandbind:create', async ({ command, action }: { command: string; action: string }) => {
    try {
      const bind = await BindService.createCommandbind(socket.user.id, command, action);
      socket.emit('commandbind:created', bind);
    } catch (error: any) {
      socket.emit('error', { message: error.message });
    }
  });

  socket.on('commandbind:update', async ({ index, command, action }: { index: number; command: string; action: string }) => {
    try {
      const bind = await BindService.updateCommandbind(socket.user.id, index, command, action);
      socket.emit('commandbind:updated', { index, bind });
    } catch (error: any) {
      socket.emit('error', { message: error.message });
    }
  });

  // Posbinds
  socket.on('posbind:create', async ({ position, action }: { position: { x: number; y: number; z: number }; action: string }) => {
    try {
      const bind = await BindService.createPosbind(socket.user.id, position, action);
      socket.emit('posbind:created', bind);
    } catch (error: any) {
      socket.emit('error', { message: error.message });
    }
  });

  socket.on('posbind:update', async ({ index, position, action }: { index: number; position: { x: number; y: number; z: number }; action: string }) => {
    try {
      const bind = await BindService.updatePosbind(socket.user.id, index, position, action);
      socket.emit('posbind:updated', { index, bind });
    } catch (error: any) {
      socket.emit('error', { message: error.message });
    }
  });

  // Allgemeine Bind-Funktionen
  socket.on('bind:delete', async ({ type, index }: { type: 'key' | 'text' | 'command' | 'pos'; index: number }) => {
    try {
      const bind = await BindService.deleteBind(socket.user.id, index, type);
      socket.emit('bind:deleted', { type, index, bind });
    } catch (error: any) {
      socket.emit('error', { message: error.message });
    }
  });

  socket.on('bind:getAll', async () => {
    try {
      const binds = await BindService.getUserBinds(socket.user.id);
      socket.emit('bind:list', binds);
    } catch (error: any) {
      socket.emit('error', { message: error.message });
    }
  });
}; 