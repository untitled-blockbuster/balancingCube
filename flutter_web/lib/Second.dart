import 'package:flutter/material.dart';

class SecondPage extends StatelessWidget {
  SecondPage({required this.message, required this.onPressBack});
  final String message;
  final Function onPressBack;

  @override
  Widget build(BuildContext context) {
    return Material(
      child: Center(
        child: Column(
          children: [
            Text(
              'Demo route handler with params:',
              style: TextStyle(fontSize: 22),
            ),
            SizedBox(
              height: 20,
            ),
            Text(
              message,
              style: TextStyle(fontSize: 18, color: Colors.blue),
            ),
            SizedBox(
              height: 20,
            ),
            FlatButton(
                onPressed: () => onPressBack(),
                color: Colors.blue,
                child: Text('Back')),
          ],
        ),
      ),
    );
  }
}
