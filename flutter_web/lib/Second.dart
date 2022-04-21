import 'package:flutter/material.dart';

class SecondPage extends StatelessWidget {
  const SecondPage({Key? key, required this.message, required this.onPressBack})
      : super(key: key);

  final String message;
  final Function onPressBack;

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      home: Scaffold(
        body: Center(
          child: Column(
            children: [
              const Text(
                'Demo route handler with params:',
                style: TextStyle(fontSize: 22),
              ),
              const SizedBox(
                height: 20,
              ),
              Text(
                message,
                style: const TextStyle(fontSize: 18, color: Colors.blue),
              ),
              const SizedBox(
                height: 20,
              ),
              ElevatedButton(
                  onPressed: () => onPressBack(), child: const Text('Back')),
            ],
          ),
        ),
      ),
    );
  }
}
