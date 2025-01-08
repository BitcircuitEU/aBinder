import mongoose from 'mongoose';
import dotenv from 'dotenv';

dotenv.config();

const MONGODB_URI = process.env.MONGODB_URI || 'mongodb://127.0.0.1:27017/abinder';

export const connectDB = async () => {
    try {
        await mongoose.connect(MONGODB_URI, {
            family: 4, // Erzwinge IPv4
            serverSelectionTimeoutMS: 5000 // Timeout nach 5 Sekunden
        });
        console.log('MongoDB Verbindung hergestellt');
    } catch (error) {
        console.error('MongoDB Verbindungsfehler:', error);
        process.exit(1);
    }
}; 