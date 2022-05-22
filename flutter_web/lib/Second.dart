import 'package:flutter/material.dart';
import 'router.dart';

class SecondPage extends StatelessWidget {
  const SecondPage({Key? key, required this.message}) : super(key: key);

  final String message;

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
                  onPressed: () {
                    Application.router.navigateTo(context, "/");
                  },
                  child: const Text('Back')),
            ],
          ),
        ),
      ),
    );
  }
}
