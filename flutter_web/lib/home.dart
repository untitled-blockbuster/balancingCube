import 'package:flutter/material.dart';
import 'router.dart';

class HomePage extends StatelessWidget {
  HomePage({Key? key}) : super(key: key);
  Size _screenSize = Size.zero;

  @override
  Widget build(BuildContext context) {
    if (_screenSize == Size.zero) {
      _screenSize = MediaQuery.of(context).size;
    }
    return Container(
      color: Colors.white,
      child: Stack(
        children: [
          Container(
            color: Colors.brown,
            margin: const EdgeInsetsDirectional.only(top: 30, start: 30),
            child: const Center(
                child: Text("test container 컨테이너"), heightFactor: 1, widthFactor: 3),
          ),
          DefaultTextStyle(
              style: const TextStyle(fontSize: 14, color: Colors.cyan),
              child: ListView(
                children: [for (int i = 0; i < 100; i++) Text(i.toString())],
              ),
              ),
          ElevatedButton(
              onPressed: () => {print(MediaQuery.of(context).size)},
              child: const Text("print screen size")),
          Container(
            color: Colors.cyan,
            margin: EdgeInsets.only(top: 30),
            child: ElevatedButton(
                onPressed: () {
                  Application.router.navigateTo(context, "/second");
                },
                child: const Text("to Second page")),
          ),
          Card(
            margin: const EdgeInsetsDirectional.only(top: 70, start: 30),
            child: InkWell(
              mouseCursor: MaterialStateMouseCursor.textable,
              child: Text("Hover"),
              onTap: () {},
              onHover: (bool a) {
                print(a);
              },
              hoverColor: Colors.red,
            ),
          ),
          // ElevatedButton(onPressed: onPressRouteButton, child: Text("link")),
          startAnimation(_screenSize)
        ],
      ),
    );
  }
}

Widget startAnimation(Size screenSize) {
  return Stack(
    children: [
      Container(
          alignment: Alignment.topLeft,
          child: StartAnimationBlock(boxWidth: screenSize.width * 0.32)),
      Container(
          alignment: Alignment.topRight,
          child: StartAnimationBlock(boxWidth: screenSize.width * 0.72))
    ],
  );
}

class StartAnimationBlock extends StatefulWidget {
  StartAnimationBlock({Key? key, required this.boxWidth}) : super(key: key);

  double boxWidth;

  @override
  State<StartAnimationBlock> createState() => _StartAnimationBlockState();
}

class _StartAnimationBlockState extends State<StartAnimationBlock>
    with SingleTickerProviderStateMixin {
  late AnimationController _controller;
  late Animation<Size> _widthAnimation;

  @override
  void initState() {
    super.initState();
    _controller =
        AnimationController(vsync: this, duration: const Duration(seconds: 2));
    _widthAnimation = Tween<Size>(
            begin: Size(widget.boxWidth, 3000), end: const Size(0, 3000))
        .animate(
            CurvedAnimation(parent: _controller, curve: Curves.easeInOutSine));

    _widthAnimation.addListener(() {
      setState(() {});
    });
  }

  @override
  void dispose() {
    _controller.dispose();
    super.dispose();
  }

  @override
  Widget build(BuildContext context) {
    _controller.forward();
    return Container(
      color: const Color.fromARGB(150, 255, 255, 255),
      height: _widthAnimation.value.height,
      width: _widthAnimation.value.width,
    );
  }
}
