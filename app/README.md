# App (Flutter web)

The Kayak Asset Manager web app. All the app code is in [`lib/main.dart`](lib/main.dart).

## Run it

1. Install Flutter (put the SDK in a folder **without spaces**, e.g. `C:\src\flutter`).
2. In this `app` folder, connect your own Firebase project (this creates `lib/firebase_options.dart`, which is not uploaded to GitHub):

   ```powershell
   dart pub global run flutterfire_cli:flutterfire configure
   ```

3. Get the packages and run:

   ```powershell
   flutter pub add firebase_core cloud_firestore
   flutter pub get
   flutter run -d chrome
   ```

## Change the lists

The gear types, activities and racks are at the top of `lib/main.dart` under **Settings you can change**.

## Put it online

```powershell
flutter build web
firebase deploy --only hosting
```

Full step-by-step guide: [`../docs/TUTORIAL.md`](../docs/TUTORIAL.md).
