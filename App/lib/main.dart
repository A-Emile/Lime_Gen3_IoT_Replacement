import 'package:dynamic_color/dynamic_color.dart';
import 'package:flutter/material.dart';
import 'package:flutter_reactive_ble/flutter_reactive_ble.dart';
import 'package:get/get.dart';
import 'package:limer/screens/devices_screen.dart';
import 'package:limer/src/ble/ble_device_connector.dart';
import 'package:limer/src/ble/ble_device_interactor.dart';
import 'package:limer/src/ble/ble_scanner.dart';
import 'package:limer/src/ble/ble_status_monitor.dart';
import 'package:limer/src/ui/ble_status_screen.dart';
import 'package:limer/src/ui/device_list.dart';
import 'package:provider/provider.dart';

import 'src/ble/ble_logger.dart';

void main() {
  WidgetsFlutterBinding.ensureInitialized();

  final _ble = FlutterReactiveBle();
  final _bleLogger = BleLogger(ble: _ble);
  final _scanner = BleScanner(ble: _ble, logMessage: _bleLogger.addToLog);
  final _monitor = BleStatusMonitor(_ble);
  final _connector = BleDeviceConnector(
    ble: _ble,
    logMessage: _bleLogger.addToLog,
  );
  final _serviceDiscoverer = BleDeviceInteractor(
    bleDiscoverServices: _ble.discoverServices,
    readCharacteristic: _ble.readCharacteristic,
    writeWithResponse: _ble.writeCharacteristicWithResponse,
    writeWithOutResponse: _ble.writeCharacteristicWithoutResponse,
    subscribeToCharacteristic: _ble.subscribeToCharacteristic,
    logMessage: _bleLogger.addToLog,
  );
  runApp(
    MultiProvider(
      providers: [
        Provider.value(value: _scanner),
        Provider.value(value: _monitor),
        Provider.value(value: _connector),
        Provider.value(value: _serviceDiscoverer),
        Provider.value(value: _bleLogger),
        StreamProvider<BleScannerState?>(
          create: (_) => _scanner.state,
          initialData: const BleScannerState(
            discoveredDevices: [],
            scanIsInProgress: false,
          ),
        ),
        StreamProvider<BleStatus?>(
          create: (_) => _monitor.state,
          initialData: BleStatus.unknown,
        ),
        StreamProvider<ConnectionStateUpdate>(
          create: (_) => _connector.state,
          initialData: const ConnectionStateUpdate(
            deviceId: 'Unknown device',
            connectionState: DeviceConnectionState.disconnected,
            failure: null,
          ),
        ),
      ],
      child: const MyApp(),
    ),
  );
}

class MyApp extends StatelessWidget {
  const MyApp({Key? key}) : super(key: key);

  static final _defaultLightColorScheme =
      ColorScheme.fromSwatch(primarySwatch: Colors.blue);

  static final _defaultDarkColorScheme = ColorScheme.fromSwatch(
      primarySwatch: Colors.blue, brightness: Brightness.dark);

  @override
  Widget build(BuildContext context) {
    final textTheme = Theme.of(context).textTheme;
    return DynamicColorBuilder(
        builder: (ColorScheme? lightDynamic, ColorScheme? darkDynamic) {
      return GetMaterialApp(
        theme: ThemeData(
          colorScheme: lightDynamic ?? _defaultLightColorScheme,
          useMaterial3: true,
        ),
        darkTheme: ThemeData(
          colorScheme: darkDynamic ?? _defaultDarkColorScheme,
          scaffoldBackgroundColor: darkDynamic?.background ?? Colors.black,
          useMaterial3: true,
        ),
        home: Scaffold(
          appBar: AppBar(
            title: const Text("Limer"),
          ),
          body: Consumer<BleStatus?>(
            builder: (_, status, __) {
              if (status == BleStatus.ready) {
                return const DeviceListScreen();
              } else {
                return BleStatusScreen(status: status ?? BleStatus.unknown);
              }
            },
          ),
        ),
      );
    });
  }
}
