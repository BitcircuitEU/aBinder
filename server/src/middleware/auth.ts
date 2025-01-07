import { Request, Response, NextFunction } from 'express';
import { Socket } from 'socket.io';
import { AuthService } from '../services/authService';

// Express Middleware
export const authenticateRequest = async (
  req: Request & { user?: any },
  res: Response,
  next: NextFunction
): Promise<void> => {
  try {
    const token = req.headers.authorization?.split(' ')[1];
    if (!token) {
      res.status(401).json({ error: 'Kein Token bereitgestellt' });
      return;
    }

    const user = await AuthService.validateToken(token);
    req.user = user;
    next();
  } catch (error) {
    res.status(401).json({ error: 'Ungültiger Token' });
  }
};

// Socket.IO Middleware
export const authenticateSocket = async (
  socket: Socket & { user?: any },
  next: (err?: Error) => void
): Promise<void> => {
  try {
    const token = socket.handshake.auth.token;
    if (!token) {
      next(new Error('Kein Token bereitgestellt'));
      return;
    }

    const user = await AuthService.validateToken(token);
    socket.user = user;
    next();
  } catch (error) {
    next(new Error('Ungültiger Token'));
  }
};

// Berechtigungsprüfung für Binder-Ränge
export const checkBinderRank = (minRank: number) => {
  return (req: Request & { user?: any }, res: Response, next: NextFunction) => {
    if (!req.user) {
      res.status(401).json({ error: 'Nicht authentifiziert' });
      return;
    }

    if (req.user.binderRank < minRank) {
      res.status(403).json({ error: 'Unzureichende Berechtigungen' });
      return;
    }

    next();
  };
}; 