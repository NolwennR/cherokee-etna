(function(factory) {
  if (typeof module === "object" && typeof module.exports === "object") {
    module.exports = factory()
  } else {
    factory()
  }
})(function() {
  var ua = window.navigator.userAgent.toLowerCase();
  var html = document.getElementsByTagName("html")[0];
  window.Platform = {
    isHD: window.devicePixelRatio > 1,
    isiPad: ua.match(/ipad/i) !== null,
    isNexus7: ua.match(/Nexus 7/gi) !== null,
    isMobile: ua.match(/Android|webOS|iPhone|iPod|BlackBerry|IEMobile/i) !== null && ua.match(/Mobile/i) !== null,
    isiPhone: ua.match(/iphone/i) !== null,
    isAndroid: ua.match(/android/i) !== null,
    isS3: ua.match(/gt\-i9300/i) !== null,
    isS4: ua.match(/(gt\-i95)|(sph\-l720)/i) !== null,
    isS5: ua.match(/sm\-g900/i) !== null,
    isIE: /(msie|trident|Edge)/i.test(navigator.userAgent),
    isIE11: ua.match(/Trident\/7\.0/i) !== null,
    isChrome: ua.match(/Chrome/gi) !== null,
    isFirefox: ua.match(/firefox/gi) !== null,
    hasTouch: "ontouchstart" in window,
    supportsSvg: !!document.createElementNS && !!document.createElementNS("http://www.w3.org/2000/svg", "svg").createSVGRect
  };
  window.Platform.isAndroidPad = Platform.isAndroid && !Platform.isMobile;
  window.Platform.isTablet = Platform.isiPad || Platform.isAndroidPad;
  window.Platform.isDesktop = !(Platform.isMobile || Platform.isTablet);
  window.Platform.isIOS = Platform.isiPad || Platform.isiPhone;
  window.Platform.isSafari = Platform.isDesktop && !Platform.isIE && !Platform.isChrome && !Platform.isFirefox;
  if (Platform.isHD) html.classList.add("hd");
  if (Platform.isiPad) html.classList.add("ipad");
  if (Platform.isNexus7) html.classList.add("nexus7");
  if (Platform.isMobile) html.classList.add("mobile");
  if (Platform.isiPhone) html.classList.add("iphone");
  if (Platform.isAndroid) html.classList.add("android");
  if (Platform.isS3) html.classList.add("s3");
  if (Platform.isS4) html.classList.add("s4");
  if (Platform.isS5) html.classList.add("s5");
  if (Platform.isIE) html.classList.add("ie");
  if (Platform.isIE11) html.classList.add("ie11");
  if (Platform.isChrome) html.classList.add("chrome");
  if (Platform.isFirefox) html.classList.add("firefox");
  if (Platform.hasTouch) html.classList.add("has-touch");
  if (!Platform.hasTouch) html.classList.add("no-touch");
  if (Platform.supportsSvg) html.classList.add("support-svg");
  if (Platform.isAndroidPad) html.classList.add("android-pad");
  if (Platform.isTablet) html.classList.add("tablet");
  if (Platform.isDesktop) html.classList.add("desktop");
  if (Platform.isIOS) html.classList.add("ios");
  if (Platform.isSafari) html.classList.add("safari");
  return window.Platform;
});