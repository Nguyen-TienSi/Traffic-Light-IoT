import { View, Text, StyleSheet } from 'react-native'
import React from 'react'

const styles = StyleSheet.create({
    container: {
        flex: 1,
        flexDirection: 'column',
        justifyContent: 'space-between',
        alignItems: 'center'
    },
    trafficLight: {
        flexDirection: 'column',
        alignItems: 'center',
    },
    light: {
        width: 50,
        height: 50,
        borderRadius: 25,
        margin: 5,
    },
    red: { backgroundColor: 'red' },
    yellow: { backgroundColor: 'yellow' },
    green: { backgroundColor: 'green' },
    counter: { fontSize: 50, marginTop: 10 },
})

const TrafficLight = () => {
    return (
        <View style={styles.container}>
            <View style={styles.trafficLight}>
                <View style={[styles.light, styles.red]} />
                <View style={[styles.light, styles.yellow]} />
                <View style={[styles.light, styles.green]} />
            </View>
            <Text style={styles.counter}>30</Text>
        </View>
    );
}

export default TrafficLight
