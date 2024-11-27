import { View, Text, Button, StyleSheet } from 'react-native';
import React from 'react';
import TrafficLight from './TrafficLight';

const styles = StyleSheet.create({
  container: {
    flex: 1,
    justifyContent: 'space-between',
    alignItems: 'center',
    flexDirection: 'column',
    marginVertical: 100,
  },
  trafficLightSection: {
    flexDirection: 'row',
    justifyContent: 'space-around',
    width: '100%',
  },
  buttonSection: {
  },
  buttonContainer: {
    marginBottom: 20,
    width: '70%',
  },
});

const App = () => {
  const featureList = [
    { command: 1, name: 'Stop counting' },
    { command: 2, name: 'Night' },
    { command: 3, name: 'Normal' },
    { command: 4, name: 'Peak hour' },
    { command: 5, name: 'Manual' },
  ];

  const sendCommand = async (command) => {
    try {
      const response = await fetch('http://192.168.2.9:5000/control', {
        method: 'POST',
        headers: {
          'Content-Type': 'application/json',
        },
        body: JSON.stringify({ command }),
      });

      if (!response.ok) {
        throw new Error(`HTTP error! status: ${response.status}`);
      }

      const data = await response.json();
      console.log('Success:', data);
    } catch (error) {
      console.error('Error:', error);
    }
  };

  return (
    <View style={styles.container}>
      <View style={styles.trafficLightSection}>
        <TrafficLight/>
        <TrafficLight/>
      </View>
      <View style={styles.buttonSection}>
        {featureList.map((feature, index) => (
          <View style={styles.buttonContainer} key={index}>
            <Button
              onPress={() => sendCommand(feature.command.toString())}
              title={feature.name}
            />
          </View>
        ))}
      </View>
    </View>
  );
};

export default App;
