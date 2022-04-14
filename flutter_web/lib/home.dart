import 'package:flutter/material.dart';
import 'package:flutter_web/routes.dart';

class HomePage extends StatelessWidget {
  const HomePage({Key? key}): super(key: key);

  Future onPressRouteButton() => locator<NavigationService>().navigateTo(SecondPageRoute, queryParams: {"message": "querystring test"});

  @override
  Widget build(BuildContext context) {
    return Material(
      child: Stack(
        children: [
          Container(
            color: Colors.red,
            margin: EdgeInsetsDirectional.only(top: 30),
            child: Center(child: Text("test container"), heightFactor: 1, widthFactor: 3),
          ),
          ListView(children: [for(int i = 0; i < 100; i++) Text(i.toString())],),
          ElevatedButton(onPressed: onPressRouteButton, child: Text("link"))
        ],
      )
    );
  }
}
