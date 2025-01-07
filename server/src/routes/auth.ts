import { Router } from 'express';
import { AuthService } from '../services/authService';

const router = Router();

router.post('/register', async (req, res) => {
  try {
    const { username, password } = req.body;
    
    if (!username || !password) {
      return res.status(400).json({ 
        error: 'Benutzername und Passwort sind erforderlich' 
      });
    }

    const { user, token } = await AuthService.register(username, password);
    res.json({ 
      message: 'Registrierung erfolgreich',
      user: {
        id: user.id,
        username: user.username,
        binderRank: user.binderRank
      },
      token 
    });
  } catch (error: any) {
    res.status(400).json({ error: error.message });
  }
});

router.post('/login', async (req, res) => {
  try {
    const { username, password } = req.body;
    
    if (!username || !password) {
      return res.status(400).json({ 
        error: 'Benutzername und Passwort sind erforderlich' 
      });
    }

    const { user, token } = await AuthService.login(username, password);
    res.json({ 
      message: 'Login erfolgreich',
      user: {
        id: user.id,
        username: user.username,
        binderRank: user.binderRank,
        groupId: user.groupId,
        factionId: user.factionId
      },
      token 
    });
  } catch (error: any) {
    res.status(401).json({ error: error.message });
  }
});

export default router; 