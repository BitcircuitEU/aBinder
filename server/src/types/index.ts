export interface User {
  id: string;
  username: string;
  password?: string;
  level?: number;
  partner?: string | { type: string; ref: 'User' };
  phoneNumber?: string;
  isBanned: boolean;
  binderRank: number; // 0: User, 1: Moderator, 2: Admin
  groupId: string;
  factionId: string;
  
  // Overlay Einstellungen
  overlaySettings: {
    inventory: {
      enabled: boolean;
      position: { x: number; y: number };
      fontSize: number;
      fontColor: string;
    };
    useTimer: {
      enabled: boolean;
      position: { x: number; y: number };
      fontSize: number;
      fontColor: string;
    };
    memberGroup: {
      enabled: boolean;
      position: { x: number; y: number };
      fontSize: number;
      fontColor: string;
    };
    memberFaction: {
      enabled: boolean;
      position: { x: number; y: number };
      fontSize: number;
      fontColor: string;
    };
    enemyFaction: {
      enabled: boolean;
      position: { x: number; y: number };
      fontSize: number;
      fontColor: string;
      targetFactionId: string;
    };
    notes: {
      enabled: boolean;
      position: { x: number; y: number };
      fontSize: number;
      fontColor: string;
      personalNotes: string[];
      groupNotes: string[];
      factionNotes: string[];
    };
    carDL: {
      enabled: boolean;
      position: { x: number; y: number };
      fontSize: number;
      fontColor: string;
    };
    sprintBar: {
      enabled: boolean;
      position: { x: number; y: number };
      fontSize: number;
      fontColor: string;
    };
  };
  
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
  groupBindTriggers: Array<{
    groupBindId: string;
    trigger: string;
  }>;
  factionBindTriggers: Array<{
    factionBindId: string;
    trigger: string;
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
  bindActions: Array<{
    name: string;
    action: string;
    minRank: number;
  }>;
}

export interface Group {
  id: string;
  name: string;
  members: {
    userId: string;
    rank: number;
    isLeader: boolean;
  }[];
  bindActions: Array<{
    name: string;
    action: string;
    minRank: number;
  }>;
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