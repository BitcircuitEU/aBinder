import bcrypt from 'bcrypt';
import jwt from 'jsonwebtoken';
import { User } from '../models';
import type { User as UserType } from '../types';

export class AuthService {
  private static readonly JWT_SECRET = process.env.JWT_SECRET || 'default-secret';
  private static readonly SALT_ROUNDS = 10;

  static async register(username: string, password: string): Promise<{ user: UserType; token: string }> {
    const existingUser = await User.findOne({ username });
    if (existingUser) {
      throw new Error('Benutzername bereits vergeben');
    }

    const hashedPassword = await bcrypt.hash(password, this.SALT_ROUNDS);
    const user = await User.create({
      username,
      password: hashedPassword,
      binderRank: 0,
      isBanned: false,
      groupId: '0',
      factionId: '0'
    });

    const token = this.generateToken(user);
    return { user, token };
  }

  static async login(username: string, password: string): Promise<{ user: UserType; token: string }> {
    const user = await User.findOne({ username });
    if (!user) {
      throw new Error('Benutzer nicht gefunden');
    }

    if (user.isBanned) {
      throw new Error('Dieser Account wurde gesperrt');
    }

    if (!user.password) {
      throw new Error('Kein Passwort für diesen Account gesetzt');
    }

    const isValidPassword = await bcrypt.compare(password, user.password);
    if (!isValidPassword) {
      throw new Error('Falsches Passwort');
    }

    const token = this.generateToken(user);
    return { user, token };
  }

  static async validateToken(token: string): Promise<UserType> {
    try {
      const decoded = jwt.verify(token, this.JWT_SECRET) as { id: string };
      const user = await User.findById(decoded.id);
      
      if (!user) {
        throw new Error('Benutzer nicht gefunden');
      }

      if (user.isBanned) {
        throw new Error('Dieser Account wurde gesperrt');
      }

      return user;
    } catch (error) {
      throw new Error('Ungültiger Token');
    }
  }

  private static generateToken(user: UserType): string {
    return jwt.sign({ id: user.id }, this.JWT_SECRET, {
      expiresIn: '30d'
    });
  }
} 