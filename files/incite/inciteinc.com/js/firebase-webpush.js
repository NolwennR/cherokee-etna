// [START get_messaging_object]
// Retrieve Firebase Messaging object.
const messaging = firebase.messaging()
// [END get_messaging_object]

// [START set_public_vapid_key]
// Add the public key generated from the console here.
messaging.usePublicVapidKey('BGJBmzX580ojFnD895Br4IUNku4zzGspNIrIgEDymqu7UEqljGThuvsOxT4UUIhqUw9zt8ekTGOcin2yIYpBXpw')
// [END set_public_vapid_key]

// IDs of divs that display Instance ID token UI or request permission UI.
const tokenDivId = 'token_div'
const permissionDivId = 'permission_div'

// [START refresh_token]
// Callback fired if Instance ID token is updated.
messaging.onTokenRefresh(() => {
  messaging
    .getToken()
    .then(refreshedToken => {
      console.log('Token refreshed.', refreshedToken)
    })
    .catch(err => {
      console.log('Unable to retrieve refreshed token ', err)
      showToken('Unable to retrieve refreshed token ', err)
    })
})
// [END refresh_token]

// [START receive_message]
// Handle incoming messages. Called when:
// - a message is received while the app has focus
// - the user clicks on an app notification created by a service worker
//   `messaging.setBackgroundMessageHandler` handler.
messaging.onMessage(payload => {
  console.log('Message received. ', payload)
})
// [END receive_message]

function initWebPush() {
  // [START get_token]
  // Get Instance ID token. Initially this makes a network call, once retrieved
  // subsequent calls to getToken will return from cache.

  // navigator.serviceWorker.register('/js/firebase-messaging-sw.js')
  // .then(registration => {
  //   console.log('registration', registration);
  //   messaging.useServiceWorker(registration);

  //     // initializePush()
  // })
  // .catch(err => console.log('Service Worker Error', err))

  messaging
    .getToken()
    .then(currentToken => {
      if (currentToken) {
        document.querySelector('#token').value = currentToken;
        submitMainForm();
      } else {
        // Show permission request.
        console.log('No Instance ID token available. Request permission to generate one.')
        // Show permission UI.
        // updateUIForPushPermissionRequired()
      }
    })
    .catch(err => {
      console.log('An error occurred while retrieving token. ', err)
    })
  // [END get_token]
}


function deleteToken() {
  // Delete Instance ID token.
  // [START delete_token]
  messaging
    .getToken()
    .then(currentToken => {
      messaging
        .deleteToken(currentToken)
        .then(() => {
          console.log('Token deleted.')
          // [START_EXCLUDE]
          // Once token is deleted update UI.
          resetUI()
          // [END_EXCLUDE]
        })
        .catch(err => {
          console.log('Unable to delete token. ', err)
        })
      // [END delete_token]
    })
    .catch(err => {
      console.log('Error retrieving Instance ID token. ', err)
      showToken('Error retrieving Instance ID token. ', err)
    })
}

// initWebPush()
