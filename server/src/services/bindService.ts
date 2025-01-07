import { User } from '../models';
import type { User as UserType } from '../types';

export class BindService {
  // Keybind Funktionen
  static async createKeybind(userId: string, trigger: string, action: string) {
    const user = await User.findById(userId);
    if (!user) {
      throw new Error('Benutzer nicht gefunden');
    }

    // Prüfe auf Konflikte
    await this.checkTriggerConflicts(user, trigger);
    
    user.keybinds.push({ trigger, action });
    await user.save();
    
    return user.keybinds[user.keybinds.length - 1];
  }

  static async updateKeybind(userId: string, bindIndex: number, trigger: string, action: string) {
    const user = await User.findById(userId);
    if (!user) {
      throw new Error('Benutzer nicht gefunden');
    }

    if (bindIndex < 0 || bindIndex >= user.keybinds.length) {
      throw new Error('Keybind nicht gefunden');
    }

    if (user.keybinds[bindIndex].trigger !== trigger) {
      await this.checkTriggerConflicts(user, trigger);
    }

    user.keybinds[bindIndex] = { trigger, action };
    await user.save();
    
    return user.keybinds[bindIndex];
  }

  // Textbind Funktionen
  static async createTextbind(userId: string, trigger: string, action: string) {
    const user = await User.findById(userId);
    if (!user) {
      throw new Error('Benutzer nicht gefunden');
    }

    user.textbinds.push({ trigger, action });
    await user.save();
    
    return user.textbinds[user.textbinds.length - 1];
  }

  static async updateTextbind(userId: string, bindIndex: number, trigger: string, action: string) {
    const user = await User.findById(userId);
    if (!user) {
      throw new Error('Benutzer nicht gefunden');
    }

    if (bindIndex < 0 || bindIndex >= user.textbinds.length) {
      throw new Error('Textbind nicht gefunden');
    }

    user.textbinds[bindIndex] = { trigger, action };
    await user.save();
    
    return user.textbinds[bindIndex];
  }

  // Commandbind Funktionen
  static async createCommandbind(userId: string, command: string, action: string) {
    const user = await User.findById(userId);
    if (!user) {
      throw new Error('Benutzer nicht gefunden');
    }

    user.commandbinds.push({ command, action });
    await user.save();
    
    return user.commandbinds[user.commandbinds.length - 1];
  }

  static async updateCommandbind(userId: string, bindIndex: number, command: string, action: string) {
    const user = await User.findById(userId);
    if (!user) {
      throw new Error('Benutzer nicht gefunden');
    }

    if (bindIndex < 0 || bindIndex >= user.commandbinds.length) {
      throw new Error('Commandbind nicht gefunden');
    }

    user.commandbinds[bindIndex] = { command, action };
    await user.save();
    
    return user.commandbinds[bindIndex];
  }

  // Posbind Funktionen
  static async createPosbind(userId: string, position: { x: number; y: number; z: number }, action: string) {
    const user = await User.findById(userId);
    if (!user) {
      throw new Error('Benutzer nicht gefunden');
    }

    user.posbinds.push({ position, action });
    await user.save();
    
    return user.posbinds[user.posbinds.length - 1];
  }

  static async updatePosbind(userId: string, bindIndex: number, position: { x: number; y: number; z: number }, action: string) {
    const user = await User.findById(userId);
    if (!user) {
      throw new Error('Benutzer nicht gefunden');
    }

    if (bindIndex < 0 || bindIndex >= user.posbinds.length) {
      throw new Error('Posbind nicht gefunden');
    }

    user.posbinds[bindIndex] = { position, action };
    await user.save();
    
    return user.posbinds[bindIndex];
  }

  // Hilfsfunktionen
  private static async checkTriggerConflicts(user: UserType, trigger: string) {
    // Prüfe Keybinds
    const hasKeybindConflict = user.keybinds.some(bind => bind.trigger === trigger);
    if (hasKeybindConflict) {
      throw new Error('Dieser Trigger ist bereits als Keybind belegt');
    }
  }

  // Bind-Listen abrufen
  static async getUserBinds(userId: string) {
    const user = await User.findById(userId);
    if (!user) {
      throw new Error('Benutzer nicht gefunden');
    }

    return {
      keybinds: user.keybinds,
      textbinds: user.textbinds,
      commandbinds: user.commandbinds,
      posbinds: user.posbinds
    };
  }

  // Bind löschen
  static async deleteBind(userId: string, bindIndex: number, type: 'key' | 'text' | 'command' | 'pos') {
    const user = await User.findById(userId);
    if (!user) {
      throw new Error('Benutzer nicht gefunden');
    }

    let deletedBind;
    switch (type) {
      case 'key':
        if (bindIndex < 0 || bindIndex >= user.keybinds.length) {
          throw new Error('Keybind nicht gefunden');
        }
        deletedBind = user.keybinds.splice(bindIndex, 1)[0];
        break;
      case 'text':
        if (bindIndex < 0 || bindIndex >= user.textbinds.length) {
          throw new Error('Textbind nicht gefunden');
        }
        deletedBind = user.textbinds.splice(bindIndex, 1)[0];
        break;
      case 'command':
        if (bindIndex < 0 || bindIndex >= user.commandbinds.length) {
          throw new Error('Commandbind nicht gefunden');
        }
        deletedBind = user.commandbinds.splice(bindIndex, 1)[0];
        break;
      case 'pos':
        if (bindIndex < 0 || bindIndex >= user.posbinds.length) {
          throw new Error('Posbind nicht gefunden');
        }
        deletedBind = user.posbinds.splice(bindIndex, 1)[0];
        break;
    }

    await user.save();
    return deletedBind;
  }
} 