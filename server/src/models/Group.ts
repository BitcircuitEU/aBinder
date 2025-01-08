import mongoose from 'mongoose';
import { Group } from '../types';

const groupBindActionSchema = {
  name: { type: String, required: true },
  action: { type: String, required: true },
  minRank: { type: Number, default: 0 }
};

const groupSchema = new mongoose.Schema<Group>({
  name: { type: String, required: true },
  members: [{
    userId: { type: String, required: true },
    rank: { type: Number, default: 0 },
    isLeader: { type: Boolean, default: false }
  }],
  bindActions: {
    type: [groupBindActionSchema],
    default: []
  }
}, {
  timestamps: true
});

export default mongoose.model<Group>('Group', groupSchema); 