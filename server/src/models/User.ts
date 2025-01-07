import mongoose from 'mongoose';
import { User } from '../types';

const MAX_BINDS = 20;

const bindSchema = {
  trigger: { type: String, required: true },
  action: { type: String, required: true }
};

const commandBindSchema = {
  command: { type: String, required: true },
  action: { type: String, required: true }
};

const posBindSchema = {
  position: {
    x: { type: Number, required: true },
    y: { type: Number, required: true },
    z: { type: Number, required: true }
  },
  action: { type: String, required: true }
};

const userSchema = new mongoose.Schema<User>({
  username: { type: String, required: true, unique: true },
  password: { type: String },
  level: { type: Number },
  partner: { type: String },
  phoneNumber: { type: String },
  isBanned: { type: Boolean, default: false },
  binderRank: { type: Number, default: 0 },
  groupId: { type: String, default: '0' },
  factionId: { type: String, default: '0' },
  
  // Binds
  keybinds: {
    type: [bindSchema],
    default: [],
    validate: [
      (val: any[]) => val.length <= MAX_BINDS,
      `Maximale Anzahl von ${MAX_BINDS} Keybinds überschritten`
    ]
  },
  textbinds: {
    type: [bindSchema],
    default: [],
    validate: [
      (val: any[]) => val.length <= MAX_BINDS,
      `Maximale Anzahl von ${MAX_BINDS} Textbinds überschritten`
    ]
  },
  commandbinds: {
    type: [commandBindSchema],
    default: [],
    validate: [
      (val: any[]) => val.length <= MAX_BINDS,
      `Maximale Anzahl von ${MAX_BINDS} Commandbinds überschritten`
    ]
  },
  posbinds: {
    type: [posBindSchema],
    default: [],
    validate: [
      (val: any[]) => val.length <= MAX_BINDS,
      `Maximale Anzahl von ${MAX_BINDS} Posbinds überschritten`
    ]
  }
}, {
  timestamps: true
});

export default mongoose.model<User>('User', userSchema); 