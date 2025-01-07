import { User } from '../models';
import type { PositionUpdate } from '../types';

export class PositionService {
  // Speichert die letzten Positionen aller Spieler im Arbeitsspeicher
  private static lastPositions: Map<string, PositionUpdate> = new Map();

  static async updatePosition(userId: string, update: Omit<PositionUpdate, 'userId'>): Promise<PositionUpdate> {
    const user = await User.findById(userId);
    if (!user) {
      throw new Error('Benutzer nicht gefunden');
    }

    const positionUpdate: PositionUpdate = {
      userId,
      ...update
    };

    // Position im Arbeitsspeicher aktualisieren
    this.lastPositions.set(userId, positionUpdate);

    return positionUpdate;
  }

  static async getPlayerPosition(userId: string): Promise<PositionUpdate | null> {
    return this.lastPositions.get(userId) || null;
  }

  static async getFactionMemberPositions(factionId: string): Promise<PositionUpdate[]> {
    if (factionId === '0') return [];

    const factionMembers = await User.find({ factionId });
    const positions: PositionUpdate[] = [];

    for (const member of factionMembers) {
      const position = this.lastPositions.get(member.id);
      if (position) {
        positions.push(position);
      }
    }

    return positions;
  }

  static async getGroupMemberPositions(groupId: string): Promise<PositionUpdate[]> {
    if (groupId === '0') return [];

    const groupMembers = await User.find({ groupId });
    const positions: PositionUpdate[] = [];

    for (const member of groupMembers) {
      const position = this.lastPositions.get(member.id);
      if (position) {
        positions.push(position);
      }
    }

    return positions;
  }

  // Prüft, ob ein Spieler in der Nähe einer Position ist
  static isNearPosition(
    playerPos: { x: number; y: number; z: number },
    targetPos: { x: number; y: number; z: number },
    radius: number = 3.0
  ): boolean {
    const dx = playerPos.x - targetPos.x;
    const dy = playerPos.y - targetPos.y;
    const dz = playerPos.z - targetPos.z;
    
    // Euklidische Distanz berechnen
    const distance = Math.sqrt(dx * dx + dy * dy + dz * dz);
    return distance <= radius;
  }

  // Bereinigt alte Positionen (kann periodisch aufgerufen werden)
  static cleanupOldPositions(maxAgeMs: number = 5 * 60 * 1000): void {
    const now = Date.now();
    for (const [userId, update] of this.lastPositions.entries()) {
      if (now - update.timestamp.getTime() > maxAgeMs) {
        this.lastPositions.delete(userId);
      }
    }
  }
} 