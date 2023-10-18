const express = require('express')
const router = express.Router()
const {fetch} = require('undici')


router.get('/', async(req, res) => {
    try {
        const data = await getApiData()
        res.render('index', {obj: data} )
    } catch (error) {
        console.error('Error:', error);
        res.status(500).send('An error occurred while fetching data.');
    }
})

async function getApiData() {
    try { 
        const res = await fetch('http://192.168.253.121')

        if (!res.ok) {
            throw new Error('Failed to fetch data')
        }

        const txt = await res.json()

        const data = {
            Distance : txt.variables.distance,
            sensName : txt.name,
            location : txt.variables.location,
            id: txt.id,
            hardware: txt.hardware,
        }

        console.log(txt)
        return data;

    } catch (error) {
        console.error('Error:', error)
        throw error
    }
    
}

module.exports = router