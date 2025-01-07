import mongoose from 'mongoose';
import type { IRCChannel } from '../types';

const ircChannelSchema = new mongoose.Schema<IRCChannel>({
  name: { type: String, required: true, unique: true },
  pin: { type: String, required: true },
  owner: { type: String, required: true },
  moderators: [{ type: String }],
  members: [{ type: String }],
  mutedUsers: [{ type: String }],
  bannedUsers: [{ type: String }]
}, {
  timestamps: true
});

export const IRCChannelModel = mongoose.model<IRCChannel>('IRCChannel', ircChannelSchema); 