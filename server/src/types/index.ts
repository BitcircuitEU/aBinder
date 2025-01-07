export interface User {
  id: string;
  username: string;
  password?: string;
  level?: number;
  partner?: string;
  phoneNumber?: string;
  isBanned: boolean;
  binderRank: number; // 0: User, 1: Moderator, 2: Admin
  groupId: string;
  factionId: string;
  
  // Integrierte Binds
  keybinds: Array<{
    trigger: string;
    action: string;
  }>;
  textbinds: Array<{
    trigger: string;
    action: string;
  }>;
  commandbinds: Array<{
    command: string;
    action: string;
  }>;
  posbinds: Array<{
    position: {
      x: number;
      y: number;
      z: number;
    };
    action: string;
  }>;
}

export interface PositionUpdate {
  userId: string;
  position: {
    x: number;
    y: number;
    z: number;
  };
  health: number;
  armor: number;
  isAlive: boolean;
  isInInterior: boolean;
  interiorId: number;
  isInVehicle: boolean;
  vehicleId: number;
  vehicleHealth: number;
  timestamp: Date;
}

export interface Overlay {
  userId: string;
  name: string;
  enabled: boolean;
  fontSize: number;
  fontStyle: string;
  fontColor: string;
  screenPosX: number;
  screenPosY: number;
}

export interface Faction {
  id: string;
  name: string;
  members: {
    userId: string;
    rank: number;
    isLeader: boolean;
  }[];
}

export interface Group {
  id: string;
  name: string;
  members: {
    userId: string;
    rank: number;
    isLeader: boolean;
  }[];
}

export interface IRCChannel {
  id: string;
  name: string;
  pin: string;
  owner: string;
  moderators: string[];
  members: string[];
  mutedUsers: string[];
  bannedUsers: string[];
}

export interface ChatMessage {
  id: string;
  from: string;
  to?: string; // Für private Nachrichten
  content: string;
  type: 'global' | 'private' | 'faction' | 'group' | 'irc';
  channelId?: string; // Für IRC Channels
  timestamp: Date;
} 