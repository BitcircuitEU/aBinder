import { ChatMessageModel, IRCChannelModel, User } from '../models';
import type { ChatMessage, IRCChannel, User as UserType } from '../types';
import bcrypt from 'bcrypt';

export class ChatService {
  // Globaler Chat
  static async sendGlobalMessage(from: string, content: string): Promise<ChatMessage> {
    return await ChatMessageModel.create({
      from,
      content,
      type: 'global',
      timestamp: new Date()
    });
  }

  // Privater Chat
  static async sendPrivateMessage(from: string, to: string, content: string): Promise<ChatMessage> {
    const toUser = await User.findById(to);
    if (!toUser) {
      throw new Error('Empfänger nicht gefunden');
    }

    return await ChatMessageModel.create({
      from,
      to,
      content,
      type: 'private',
      timestamp: new Date()
    });
  }

  // Fraktions-Chat
  static async sendFactionMessage(from: string, factionId: string, content: string): Promise<ChatMessage> {
    const user = await User.findById(from);
    if (!user || user.factionId !== factionId) {
      throw new Error('Keine Berechtigung für diesen Fraktions-Chat');
    }

    return await ChatMessageModel.create({
      from,
      content,
      type: 'faction',
      timestamp: new Date()
    });
  }

  // Gruppen-Chat
  static async sendGroupMessage(from: string, groupId: string, content: string): Promise<ChatMessage> {
    const user = await User.findById(from);
    if (!user || user.groupId !== groupId) {
      throw new Error('Keine Berechtigung für diesen Gruppen-Chat');
    }

    return await ChatMessageModel.create({
      from,
      content,
      type: 'group',
      timestamp: new Date()
    });
  }

  // IRC Channel Management
  static async createIRCChannel(name: string, pin: string, owner: string): Promise<IRCChannel> {
    const hashedPin = await bcrypt.hash(pin, 10);
    return await IRCChannelModel.create({
      name,
      pin: hashedPin,
      owner,
      moderators: [],
      members: [owner],
      mutedUsers: [],
      bannedUsers: []
    });
  }

  static async joinIRCChannel(channelId: string, userId: string, pin: string): Promise<void> {
    const channel = await IRCChannelModel.findById(channelId);
    if (!channel) {
      throw new Error('Channel nicht gefunden');
    }

    if (channel.bannedUsers.includes(userId)) {
      throw new Error('Du wurdest aus diesem Channel gebannt');
    }

    if (channel.members.includes(userId)) {
      throw new Error('Du bist bereits Mitglied dieses Channels');
    }

    const isValidPin = await bcrypt.compare(pin, channel.pin);
    if (!isValidPin) {
      throw new Error('Falsche PIN');
    }

    await IRCChannelModel.findByIdAndUpdate(channelId, {
      $push: { members: userId }
    });
  }

  static async sendIRCMessage(from: string, channelId: string, content: string): Promise<ChatMessage> {
    const channel = await IRCChannelModel.findById(channelId);
    if (!channel) {
      throw new Error('Channel nicht gefunden');
    }

    if (!channel.members.includes(from)) {
      throw new Error('Keine Berechtigung für diesen Channel');
    }

    if (channel.mutedUsers.includes(from)) {
      throw new Error('Du wurdest stummgeschaltet');
    }

    return await ChatMessageModel.create({
      from,
      content,
      type: 'irc',
      channelId,
      timestamp: new Date()
    });
  }

  // IRC Channel Moderation
  static async muteUser(channelId: string, userId: string, moderatorId: string): Promise<void> {
    const channel = await IRCChannelModel.findById(channelId);
    if (!channel) {
      throw new Error('Channel nicht gefunden');
    }

    if (!channel.moderators.includes(moderatorId) && channel.owner !== moderatorId) {
      throw new Error('Keine Moderationsrechte');
    }

    if (channel.owner === userId) {
      throw new Error('Der Channel-Inhaber kann nicht stummgeschaltet werden');
    }

    await IRCChannelModel.findByIdAndUpdate(channelId, {
      $addToSet: { mutedUsers: userId }
    });
  }

  static async unmuteUser(channelId: string, userId: string, moderatorId: string): Promise<void> {
    const channel = await IRCChannelModel.findById(channelId);
    if (!channel) {
      throw new Error('Channel nicht gefunden');
    }

    if (!channel.moderators.includes(moderatorId) && channel.owner !== moderatorId) {
      throw new Error('Keine Moderationsrechte');
    }

    await IRCChannelModel.findByIdAndUpdate(channelId, {
      $pull: { mutedUsers: userId }
    });
  }

  static async banUser(channelId: string, userId: string, moderatorId: string): Promise<void> {
    const channel = await IRCChannelModel.findById(channelId);
    if (!channel) {
      throw new Error('Channel nicht gefunden');
    }

    if (channel.owner !== moderatorId) {
      throw new Error('Nur der Channel-Inhaber kann Nutzer bannen');
    }

    if (channel.owner === userId) {
      throw new Error('Der Channel-Inhaber kann nicht gebannt werden');
    }

    await IRCChannelModel.findByIdAndUpdate(channelId, {
      $addToSet: { bannedUsers: userId },
      $pull: { members: userId, moderators: userId }
    });
  }

  static async promoteModerator(channelId: string, userId: string, ownerId: string): Promise<void> {
    const channel = await IRCChannelModel.findById(channelId);
    if (!channel) {
      throw new Error('Channel nicht gefunden');
    }

    if (channel.owner !== ownerId) {
      throw new Error('Nur der Channel-Inhaber kann Moderatoren ernennen');
    }

    if (!channel.members.includes(userId)) {
      throw new Error('Der Nutzer ist kein Mitglied des Channels');
    }

    await IRCChannelModel.findByIdAndUpdate(channelId, {
      $addToSet: { moderators: userId }
    });
  }

  // Chat-Historie abrufen
  static async getChannelHistory(channelId: string, userId: string, limit = 50): Promise<ChatMessage[]> {
    const channel = await IRCChannelModel.findById(channelId);
    if (!channel) {
      throw new Error('Channel nicht gefunden');
    }

    if (!channel.members.includes(userId)) {
      throw new Error('Keine Berechtigung für diesen Channel');
    }

    return await ChatMessageModel.find({
      channelId,
      type: 'irc'
    })
      .sort({ timestamp: -1 })
      .limit(limit);
  }

  static async getGlobalHistory(limit = 100): Promise<ChatMessage[]> {
    return await ChatMessageModel.find({ type: 'global' })
      .sort({ timestamp: -1 })
      .limit(limit)
      .exec();
  }

  static async getPrivateHistory(userId: string, otherUserId: string, limit = 50): Promise<ChatMessage[]> {
    return await ChatMessageModel.find({
      type: 'private',
      $or: [
        { from: userId, to: otherUserId },
        { from: otherUserId, to: userId }
      ]
    })
      .sort({ timestamp: -1 })
      .limit(limit);
  }
} 