import mongoose from 'mongoose';
import { User } from '../types';

const MAX_BINDS = 20;

const positionSchema = {
  x: { type: Number, required: true },
  y: { type: Number, required: true }
};

const overlayBaseSchema = {
  enabled: { type: Boolean, default: true },
  position: positionSchema,
  fontSize: { type: Number, default: 12 },
  fontColor: { type: String, default: '#FFFFFF' }
};

const overlaySettingsSchema = {
  inventory: overlayBaseSchema,
  useTimer: overlayBaseSchema,
  memberGroup: overlayBaseSchema,
  memberFaction: overlayBaseSchema,
  enemyFaction: {
    ...overlayBaseSchema,
    targetFactionId: { type: String, default: '0' }
  },
  notes: {
    ...overlayBaseSchema,
    personalNotes: [{ type: String }],
    groupNotes: [{ type: String }],
    factionNotes: [{ type: String }]
  },
  carDL: overlayBaseSchema,
  sprintBar: overlayBaseSchema
};

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

const groupBindTriggerSchema = {
  groupBindId: { type: mongoose.Schema.Types.ObjectId, required: true },
  trigger: { type: String, required: true }
};

const factionBindTriggerSchema = {
  factionBindId: { type: mongoose.Schema.Types.ObjectId, required: true },
  trigger: { type: String, required: true }
};

const userSchema = new mongoose.Schema<User>({
  username: { type: String, required: true, unique: true },
  password: { type: String },
  level: { type: Number },
  partner: { type: mongoose.Schema.Types.ObjectId, ref: 'User' },
  phoneNumber: { type: String },
  isBanned: { type: Boolean, default: false },
  binderRank: { type: Number, default: 0 },
  groupId: { type: String, default: '0' },
  factionId: { type: String, default: '0' },
  
  // Overlay Einstellungen
  overlaySettings: {
    type: overlaySettingsSchema,
    default: () => ({
      inventory: { enabled: true, position: { x: 10, y: 10 }, fontSize: 12, fontColor: '#FFFFFF' },
      useTimer: { enabled: true, position: { x: 10, y: 50 }, fontSize: 12, fontColor: '#FFFFFF' },
      memberGroup: { enabled: true, position: { x: 10, y: 90 }, fontSize: 12, fontColor: '#FFFFFF' },
      memberFaction: { enabled: true, position: { x: 10, y: 130 }, fontSize: 12, fontColor: '#FFFFFF' },
      enemyFaction: { enabled: true, position: { x: 10, y: 170 }, fontSize: 12, fontColor: '#FF0000', targetFactionId: '0' },
      notes: { 
        enabled: true, 
        position: { x: 10, y: 210 }, 
        fontSize: 12, 
        fontColor: '#FFFFFF',
        personalNotes: [],
        groupNotes: [],
        factionNotes: []
      },
      carDL: { enabled: true, position: { x: 10, y: 250 }, fontSize: 12, fontColor: '#FFFFFF' },
      sprintBar: { enabled: true, position: { x: 10, y: 290 }, fontSize: 12, fontColor: '#FFFFFF' }
    })
  },
  
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
  },
  groupBindTriggers: {
    type: [groupBindTriggerSchema],
    default: [],
    validate: [
      (val: any[]) => val.length <= MAX_BINDS,
      `Maximale Anzahl von ${MAX_BINDS} Gruppenbind-Trigger überschritten`
    ]
  },
  factionBindTriggers: {
    type: [factionBindTriggerSchema],
    default: [],
    validate: [
      (val: any[]) => val.length <= MAX_BINDS,
      `Maximale Anzahl von ${MAX_BINDS} Fraktionsbind-Trigger überschritten`
    ]
  }
}, {
  timestamps: true
});

export default mongoose.model<User>('User', userSchema); 