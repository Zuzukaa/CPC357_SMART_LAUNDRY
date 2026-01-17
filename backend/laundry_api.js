// 1. IMPORT TOOLS
// We use Express for the server, MongoDB to talk to our DB, and CORS so React can connect
const express = require('express');
const { MongoClient } = require('mongodb');
const cors = require('cors');

const app = express();
app.use(cors()); // Allows React app to talk to this API
app.use(express.json());

// 2. DATABASE SETTINGS
// Points to the MongoDB you installed on this GCP instance
const url = 'mongodb://localhost:27017';
const client = new MongoClient(url);
const dbName = 'smart_laundry';

// 3. THE "GET DATA" ROUTE
// When React visits http://35.208.56.72:5002/api/laundry-data, this code runs
app.get('/api/laundry-data', async (req, res) => {
    try {
        await client.connect();
        const db = client.db(dbName);
        const collection = db.collection('sensor_readings');
        
        // Fetch the 20 most recent sensor readings for the chart
        const data = await collection.find({})
            .sort({ timestamp: -1 }) 
            .limit(20)
            .toArray();
            
        res.json(data); // Send the data back to React
    } catch (err) {
        res.status(500).json({ error: "Database error" });
    }
});

// 4. START THE SERVER
// We use port 5002 
app.listen(5002, () => {
    console.log("Laundry API is live on port 5002");
});