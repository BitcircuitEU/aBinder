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
    // Get auth data from query parameters
    const token = socket.handshake.query.token as string;
    const username = socket.handshake.query.username as string;

    if (!token) {
      console.error('Kein Token in Query-Parametern gefunden');
      next(new Error('Kein Token bereitgestellt'));
      return;
    }

    const user = await AuthService.validateToken(token);

    if (!user || !user.username) {
      console.error('Ungültiger Benutzer oder kein Benutzername');
      next(new Error('Ungültiger Benutzer'));
      return;
    }

    // Verify that the username matches
    if (user.username !== username) {
      console.error('Username stimmt nicht mit Token überein:', { provided: username, token: user.username });
      next(new Error('Username stimmt nicht mit Token überein'));
      return;
    }

    socket.user = user;
    socket.data = { 
      ...socket.data,
      username: user.username,
      userId: user.id
    };

    console.log('Socket authentifiziert:', {
      socketId: socket.id,
      username: socket.data.username,
      userId: socket.data.userId
    });

    next();
  } catch (error) {
    console.error('Socket Authentifizierungsfehler:', error);
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