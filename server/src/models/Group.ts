import mongoose from 'mongoose';
import { Group } from '../types';

const groupSchema = new mongoose.Schema<Group>({
  name: { type: String, required: true },
  members: [{
    userId: { type: String, required: true },
    rank: { type: Number, default: 0 },
    isLeader: { type: Boolean, default: false }
  }]
}, {
  timestamps: true
});

export default mongoose.model<Group>('Group', groupSchema); 