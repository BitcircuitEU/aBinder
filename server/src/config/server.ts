import dotenv from 'dotenv';
import { Server } from 'socket.io';
import { createServer as createHttpsServer } from 'https';
import { Server as HttpsServer } from 'https';
import express from 'express';
import cors from 'cors';
import jwt from "jsonwebtoken";
import fs from 'fs';
import path from 'path';

// Lade Umgebungsvariablen
dotenv.config();

const JWT_SECRET = process.env.JWT_SECRET || 'default-secret';
const PORT = process.env.PORT || 443;

// SSL Zertifikate laden
const sslOptions = {
    cert: fs.readFileSync(process.env.SSL_CERT_PATH || ''),
    key: fs.readFileSync(process.env.SSL_KEY_PATH || '')
};

const app = express();
app.use(cors({
    origin: '*',
    methods: ['GET', 'POST'],
    allowedHeaders: ['Content-Type', 'Authorization'],
    credentials: true
}));
app.use(express.json());

// HTTPS Server erstellen
const server = createHttpsServer(sslOptions, app);

// Socket.IO Server erstellen
export function configureSocketServer(httpsServer: HttpsServer) {
    const io = new Server(httpsServer, {
        cors: {
            origin: "*",
            methods: ["GET", "POST"],
            credentials: true
        },
        pingInterval: 25000,
        pingTimeout: 20000,
        connectTimeout: 10000,
        transports: ["websocket"],
        allowUpgrades: false,
        perMessageDeflate: false,
        maxHttpBufferSize: 1e6
    });

    // Auth Middleware
    io.use(async (socket, next) => {
        try {
            const token = socket.handshake.query.token as string;
            const username = socket.handshake.query.username as string;

            if (!token) {
                console.log("Kein Token gefunden");
                return next(new Error("Authentication error"));
            }

            console.log("Empfangene Auth-Daten:", {
                token: token,
                username: username
            });

            const decoded = jwt.verify(token, JWT_SECRET) as { id: string };
            if (!decoded) {
                console.log("Token ungültig");
                return next(new Error("Authentication error"));
            }

            console.log("Dekodiertes Token:", decoded);

            // Hole den Benutzer aus der Datenbank
            const user = await import('../models/User').then(m => m.default.findById(decoded.id));
            if (!user) {
                console.log("Benutzer nicht gefunden");
                return next(new Error("Authentication error"));
            }

            // Verify username matches
            if (user.username !== username) {
                console.log("Username stimmt nicht mit Token überein:", {
                    provided: username,
                    stored: user.username
                });
                return next(new Error("Authentication error"));
            }

            console.log("Token validiert für Benutzer:", user.username);
            socket.data.user = user;
            socket.data.username = user.username;
            next();
        } catch (error) {
            console.log("Auth Error:", error);
            next(new Error("Authentication error"));
        }
    });

    return io;
}

// Server starten
server.listen(PORT, () => {
    console.log(`HTTPS Server läuft auf Port ${PORT} mit SSL`);
    console.log('SSL Zertifikat geladen von:', process.env.SSL_CERT_PATH);
});

const io = configureSocketServer(server);
export { app, server, io }; 