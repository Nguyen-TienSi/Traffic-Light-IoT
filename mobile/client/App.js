import { View, Text, Button, StyleSheet } from 'react-native'
import React from 'react'

const styles = StyleSheet.create({
  container: {
    flex: 1,
    justifyContent: 'center',
    alignItems: 'center',
  },
  buttonContainer: {
    marginBottom: 20,
    paddingHorizontal: 20
  },
});

const App = () => {

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
      <View style={styles.buttonContainer}>
        <Button onPress={() => sendCommand('1')} title='Feature 1' />
      </View>
      <View style={styles.buttonContainer}>
        <Button onPress={() => sendCommand('2')} title='Feature 2' />
      </View>
      <View style={styles.buttonContainer}>
        <Button onPress={() => sendCommand('3')} title='Feature 3' />
      </View>
      <View style={styles.buttonContainer}>
        <Button onPress={() => sendCommand('4')} title='Feature 4' />
      </View>
      <View style={styles.buttonContainer}>
        <Button onPress={() => sendCommand('5')} title='Feature 5' />
      </View>
    </View>
  )
}

export default App