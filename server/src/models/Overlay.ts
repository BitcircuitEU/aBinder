import mongoose from 'mongoose';
import type { Overlay } from '../types';

const overlaySchema = new mongoose.Schema<Overlay>({
  userId: { type: String, required: true },
  name: { type: String, required: true },
  enabled: { type: Boolean, default: true },
  fontSize: { type: Number, required: true },
  fontStyle: { type: String, required: true },
  fontColor: { type: String, required: true },
  screenPosX: { type: Number, required: true },
  screenPosY: { type: Number, required: true }
}, {
  timestamps: true
});

export const OverlayModel = mongoose.model<Overlay>('Overlay', overlaySchema); 