import mongoose from 'mongoose';
import { Faction } from '../types';

const factionBindActionSchema = {
  name: { type: String, required: true },
  action: { type: String, required: true },
  minRank: { type: Number, default: 0 }
};

const factionSchema = new mongoose.Schema<Faction>({
  name: { type: String, required: true },
  members: [{
    userId: { type: String, required: true },
    rank: { type: Number, default: 0 },
    isLeader: { type: Boolean, default: false }
  }],
  bindActions: {
    type: [factionBindActionSchema],
    default: []
  }
}, {
  timestamps: true
});

export default mongoose.model<Faction>('Faction', factionSchema); 