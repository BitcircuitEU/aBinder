import mongoose from 'mongoose';
import { Faction } from '../types';

const factionSchema = new mongoose.Schema<Faction>({
  name: { type: String, required: true },
  members: [{
    userId: { type: String, required: true },
    rank: { type: Number, default: 0 },
    isLeader: { type: Boolean, default: false }
  }]
}, {
  timestamps: true
});

export default mongoose.model<Faction>('Faction', factionSchema); 