import mongoose from 'mongoose';
import type { ChatMessage } from '../types';

const chatMessageSchema = new mongoose.Schema<ChatMessage>({
  from: { type: String, required: true },
  to: { type: String },
  content: { type: String, required: true },
  type: { 
    type: String, 
    required: true,
    enum: ['global', 'private', 'faction', 'group', 'irc']
  },
  channelId: { type: String },
  timestamp: { type: Date, default: Date.now }
}, {
  timestamps: true
});

export const ChatMessageModel = mongoose.model<ChatMessage>('ChatMessage', chatMessageSchema); 