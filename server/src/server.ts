import express from 'express';
import http from 'http';
import { Server, Socket } from 'socket.io';
import mongoose from 'mongoose';
import bcrypt from 'bcrypt';

// Express setup
const app = express();
const server = http.createServer(app);
const io = new Server(server, {
  cors: {
    origin: "*",
    methods: ["GET", "POST"]
  }
});

// MongoDB connection
const mongoURI = 'mongodb://127.0.0.1:27017/aBinder';
mongoose.connect(mongoURI, { useNewUrlParser: true, useUnifiedTopology: true })
  .then(() => console.log('MongoDB connected'))
  .catch(err => console.log('MongoDB connection error:', err));

// User model
const UserSchema = new mongoose.Schema({
  username: { type: String, unique: true, required: true },
  password: { type: String, required: true },
});

const User = mongoose.model('User', UserSchema);

interface LoginData {
  username: string;
  password: string;
}

io.on('connection', (socket: Socket) => {
  console.log('New client connected');

  socket.on('auth', async (data: string) => {
    console.log("Auth Triggered");
    try {
      const { username, password }: LoginData = JSON.parse(data);
      let user = await User.findOne({ username });

      if (!user) {
        // Create new user if not found
        const hashedPassword = await bcrypt.hash(password, 10);
        user = new User({ username, password: hashedPassword });
        await user.save();
        await socket.emit('loginResponse', JSON.stringify({
          success: true,
          username: user.username,
          message: 'New user created and logged in'
        }));
      } else {
        // Check password for existing user
        const isMatch = await bcrypt.compare(password, user.password);
        if (isMatch) {
          await socket.emit('loginResponse', JSON.stringify({
            success: true,
            username: user.username,
            message: 'Login successful'
          }));
        } else {
          await socket.emit('loginResponse', JSON.stringify({
            success: false,
            message: 'Invalid credentials'
          }));
        }
      }
    } catch (error) {
      console.error('Login error:', error);
      await socket.emit('loginResponse', JSON.stringify({
        success: false,
        message: 'An error occurred during login'
      }));
    }
  });

  socket.on('disconnect', () => {
    console.log('Client disconnected');
  });
});

const PORT = process.env.PORT || 4325;
server.listen(PORT, () => console.log(`Server running on port ${PORT}`));