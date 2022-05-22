import 'package:flutter/material.dart';
import 'package:flutter_web/home.dart';
import 'package:flutter_web/Second.dart';
import 'package:fluro/fluro.dart';

class RouteAnimation extends PageRouteBuilder {
  final Widget child;
  final String? routeName;

  RouteAnimation({required this.child, required this.routeName})
      : super(
          settings: RouteSettings(name: routeName),
          pageBuilder: (
            BuildContext context,
            Animation<double> animation,
            Animation<double> secondaryAnimation,
          ) =>
              child,
          transitionsBuilder: (
            BuildContext context,
            Animation<double> animation,
            Animation<double> secondaryAnimation,
            Widget child,
          ) =>
              RotationTransition(
            turns: animation,
            child: child,
          ),
        );
}

class FadeRoute extends PageRouteBuilder {
  final Widget child;
  final String? routeName;

  FadeRoute({required this.child, required this.routeName})
      : super(
          settings: RouteSettings(name: routeName),
          pageBuilder: (
            BuildContext context,
            Animation<double> animation,
            Animation<double> secondaryAnimation,
          ) =>
              child,
          transitionsBuilder: (
            BuildContext context,
            Animation<double> animation,
            Animation<double> secondaryAnimation,
            Widget child,
          ) =>
              FadeTransition(
            opacity: animation,
            child: child,
          ),
        );
}

var homePageHandler = Handler(
  handlerFunc: (BuildContext? context, Map<String, List<String>> params) {
    return HomePage();
  }
);
var secondPageHandler = Handler(
  handlerFunc: (BuildContext? context, Map<String, List<String>> params) {
    return SecondPage();
  }
);
class Routes {
  static String homePageRoute = '/';
  static String secondPageRoute = '/second';

  static void configureRoutes(FluroRouter router) {
    router.notFoundHandler = Handler(
        handlerFunc: (BuildContext? context, Map<String, List<String>> params) {
      print("ROUTE WAS NOT FOUND !!!");
      return;
    });
    router.define(homePageRoute, handler: homePageHandler, transitionDuration: Duration(seconds: 0));
    router.define(secondPageRoute, handler: secondPageHandler, transitionDuration: Duration(seconds: 0));
  }
}