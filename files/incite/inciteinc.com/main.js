$(document).ready(function() {
  var winW = window.innerWidth;
  var winH = window.innerHeight;
  var MOBILE_BR = 825;
  var particlesScaler = 3;
  var targetSection;
  var currentSurvey = 0;
  var medicalVideo = $('.medical-video')[0];
  var audioElement = document.createElement('audio');
  var isMenuAnimate = false;
  var isUserLoggedIn = false;
  var isUserCompleted = false;
  var isUserUnderage = false;
  var notifAlertClose = false;
  var scores = [];
  var INFOGRAPHIC_SCORE = {
    martyr: { min: 13, max: 17, name: 'The Martyr', img: 'incite_path_martyr_1.png', desc: '<p>You have a tendency to take personal responsibility for others, sacrificing your personal happiness to ensure the success of those around you. Your instincts and natural likability can give you great power if harnessed properly; over the coming weeks, Incite will work with you on putting those traits to work to get what you deserve.</p><p>RECOMMENDED LIMBIC TAB: <a onclick="setMedical()">Cherry Catalyst</a></p>' },
    conformist: { min: 18, max: 22, name: 'The Conformist', img: 'incite_path_conformist_r1.png', desc: '<p>You are under no delusions about the world. You’re adept at analyzing situations and determining the most probable outcomes; your vantage point allows you to see the bigger picture. Incite can help capitalize on this height advantage so you take control while others get swept into the power vacuum. There is always need for strong, rational voices like yours.</p><p>RECOMMENDED LIMBIC TAB: <a onclick="setMedical()">Soaring Eagle</a></p>' },
    realist: { min: 23, max: 27, name: 'The Realist', img: 'incite_path_realist_r1.png', desc: '<p>You easily fit into the mainstream, moving in step with the pack and enrobing yourself in the camouflage of those around you. While society often focuses on iconoclasts, our data has determined that people like you often have the highest rate of success in life; there is far greater long-term advantage to fitting in than standing out.</p><p>RECOMMENDED LIMBIC TAB: <a onclick="setMedical()">Ocean Breeze</a></p>' },
    optimist: { min: 28, max: 32, name: 'The Optimist', img: 'incite_path_optimist_r1.png', desc: '<p>You trust the process--characterized by an unwavering faith in the path of progress and an innate confidence that things will work out for the best. You are well-suited to today’s world, where Incite’s improvements are continually making the world easier, smarter, and safer for all. The best thing you could do for yourself is proselytize your optimism to others, and we’re here to help.</p><p>RECOMMENDED LIMBIC TAB: <a onclick="setMedical()">Aurora Borealis</a></p>' },
    opportunist: { min: 33, max: 37, name: 'The Opportunist', img: 'incite_path_opportunist_r1.png', desc: '<p>You are adept at making any situation work to your advantage. While this often sets you up for success in the moment, you may find yourself snaking further away from your long-term goals in the name of expedience. Incite will show you when it’s best to take a beat to consider your natural impulses--maybe there’s a greater prize to claim farther down the path.</p><p>RECOMMENDED LIMBIC TAB: <a onclick="setMedical()">Zen Garden</a></p>' },
    striver: { min: 38, max: 42, name: 'The Striver', img: 'incite_path_striver_r1.png', desc: '<p>When you speak, all people hear are the no’s, when it’s really your exacting standards that make it difficult for you to see the good in situations. Preferring to poke holes instead of mend fences may seem antithetical to success, but the world needs people unafraid to ask the tough questions. Incite can put you on the right path by aligning you with the people and situations that interact most positively with your natural cynicism.</p><p>RECOMMENDED LIMBIC TAB: <a onclick="setMedical()">Silent Night</a></p>' },
    naysayer: { min: 43, max: 47, name: 'The Naysayer', img: 'incite_path_naysayer_r1.png', desc: '<p>You don’t like to play by society’s rules, and that can sometimes get you into trouble. Your natural inclination to stand apart can either make you a leader or an outcast, and you will be best served letting others share the gift of your independent spirit. Progress starts with people like you. With Incite, channeling your drive in service of the greater good will make you happy, healthy, and wealthy beyond your wildest dreams.</p><p>RECOMMENDED LIMBIC TAB: <a onclick="setMedical()">Impulse</a></p>' },
    rebel: { min: 48, max: 52, name: 'The Rebel', img: 'incite_path_rebel_r1.png', desc: '<p>You never rest on your laurels, always pushing for the next level of success. Your natural tendencies have already set you on a good path; however, in order to truly succeed you must be able to handle the losses as well as the victories. Incite’s helpful life path algorithms can help you plot your future, but there are some things you can’t plan for. Every failure is just the launching pad for the next big win.</p><p>RECOMMENDED LIMBIC TAB: <a onclick="setMedical()">Coral Sands</a></p>' },
  };



  





  // main menu / plus animation
  var tl = new TimelineLite({ onComplete:function() {
    tl.restart();
  } });
  tl.to('.plus-0', 0.5, { onStart: function() { $('.plus-0, .plus-1, .plus-4, .plus-5').removeClass('active'); $('.plus-0').addClass('active'); } });
  tl.to('.plus-0', 0.5, { onStart: function() { $('.plus-0').removeClass('active'); }, delay: 1.5 });
  tl.to('.plus-4', 0.5, { onStart: function() { $('.plus-4').addClass('active'); } });
  tl.to('.plus-4', 0.5, { onStart: function() { $('.plus-4').removeClass('active'); }, delay: 1.5 });
  tl.to('.plus-1', 0.5, { onStart: function() { $('.plus-1').addClass('active'); } });
  tl.to('.plus-1', 0.5, { onStart: function() { $('.plus-1').removeClass('active'); }, delay: 1.5 });
  // tl.to('.plus-5', 0.5, { onStart: function() { $('.plus-5').addClass('active'); } });
  // tl.to('.plus-5', 0.5, { onStart: function() { $('.plus-5').removeClass('active'); }, delay: 1.5 });
  tl.pause();




  // intro / welcome
  var introTL = new TimelineMax();
  introTL.to('.welcome', 0.5, { autoAlpha: 0, display: 'none', delay: 1.5 });
  introTL.to('.particles-wrapper', 0.5, { autoAlpha: 1, transformOrigin: 'center', onComplete: function() {
    if (Platform.hasTouch) {
      isMenuAnimate = true;
      tl.play(0);
    }

    // show notifications on start
    showNotification();
  } });
  introTL.pause();




  // initial size of fake-shape
  // hide fakeshape on sphere view
  TweenMax.set('.fs2', { autoAlpha: 0 });

  // resize fakeshape function
  var dw = 1;
  var dh = 1;
  var multiplier = 1;
  var ticker;
  var dw2 = 1;
  var dh2 = 1;
  var multiplier2 = 1;
  var resizeFakeShape = function() {
    dw = winW;
    dh = winH;
    multiplier = (winW > MOBILE_BR ? 0.65 : 0.85);
    dw2 = winW;
    dh2 = winH;
    multiplier2 = (winW > MOBILE_BR ? 1.3 : 1.7);
    if (winW < winH) {
      dw = winW * multiplier;
      dh = winW * multiplier;
      dw2 = winW * multiplier2;
      dh2 = winW * multiplier2;
    } else {
      dw = winH * multiplier;
      dh = winH * multiplier;
      dw2 = winH * multiplier2;
      dh2 = winH * multiplier2;
    }
    
    clearTimeout(ticker);
    ticker = setTimeout(function() {
      TweenMax.set('.plus-wrapper', { width: dw, height: dh });
      TweenMax.set('.particles-video-full', { width: dw2, height: dh2 });

      var sc = $('.fake-shape').width() / $('#particles-js').width();

      if (winW > MOBILE_BR) {
        $('.plus-0').css({ 
          left: $('.plus-wrapper').position().left + $('.plus-wrapper').width() * 0.94, 
          top: $('.plus-wrapper').position().top + $('.plus-wrapper').width() * 0.28
        });
        $('.plus-5').css({ 
          left: $('.plus-wrapper').position().left + $('.plus-wrapper').width() * 0.81, 
          top: $('.plus-wrapper').position().top + $('.plus-wrapper').width() * 0.85
        });
        $('.plus-1').css({ 
          left: $('.plus-wrapper').position().left + $('.plus-wrapper').width() * 0.07, 
          top: $('.plus-wrapper').position().top + $('.plus-wrapper').width() * 0.19
        });
        $('.plus-4').css({ 
          left: $('.plus-wrapper').position().left + $('.plus-wrapper').width() * 0.25, 
          top: $('.plus-wrapper').position().top + $('.plus-wrapper').width() * 0.7
        });
      } else {
        $('.plus-0').css({ 
          left: $('.plus-wrapper').position().left + $('.plus-wrapper').width() * 0.56, 
          top: $('.plus-wrapper').position().top + $('.plus-wrapper').width() * -0.02
        });
        $('.plus-5').css({ 
          left: $('.plus-wrapper').position().left + $('.plus-wrapper').width() * 0.7, 
          top: $('.plus-wrapper').position().top + $('.plus-wrapper').width() * 0.93
        });
        $('.plus-1').css({ 
          left: $('.plus-wrapper').position().left + $('.plus-wrapper').width() * 0.08, 
          top: $('.plus-wrapper').position().top + $('.plus-wrapper').width() * 0.12
        });
        $('.plus-4').css({ 
          left: $('.plus-wrapper').position().left + $('.plus-wrapper').width() * 0.22, 
          top: $('.plus-wrapper').position().top + $('.plus-wrapper').width() * 0.72
        });
      }
    }, 50);
  }

  





  TweenMax.set('.particles-video-full', { autoAlpha: 1 });
  TweenMax.set('.particles-video-cu', { autoAlpha: 0 });
  var resetSphere = function() {
    TweenMax.to('.particles-video-full', 0.5, { autoAlpha: 1, ease: Power2.easeOut, overwrite: true });
    TweenMax.to('.cu-normal', 0.5, { autoAlpha: 0, ease: Power2.easeOut, overwrite: true });
    TweenMax.to('.cu-black', 0.5, { autoAlpha: 0, ease: Power2.easeOut, overwrite: true });
    TweenMax.to('.cu-zoom', 0.5, { autoAlpha: 0, ease: Power2.easeOut, overwrite: true });
    TweenMax.to('.particles-wrapper', 0.5, { autoAlpha: 1, ease: Power2.easeOut, overwrite: true });
    window.location.hash = '';
  };

  var placeSphereRight = function() {
    TweenMax.to('.particles-video-full', 0.5, { autoAlpha: 0, ease: Power2.easeOut, overwrite: true });
    TweenMax.to('.cu-normal', 0.5, { autoAlpha: 1, ease: Power2.easeOut, overwrite: true });
    TweenMax.to('.cu-black', 0.5, { autoAlpha: 0, ease: Power2.easeOut, overwrite: true });
    TweenMax.to('.cu-zoom', 0.5, { autoAlpha: 0, ease: Power2.easeOut, overwrite: true });
    TweenMax.to('.particles-wrapper', 0.5, { autoAlpha: 0, ease: Power2.easeOut, overwrite: true });
  };

  var placeSphereCenterScale = function() {
    TweenMax.to('.particles-video-full', 0.5, { autoAlpha: 0, ease: Power2.easeOut, overwrite: true });
    TweenMax.to('.cu-normal', 0.5, { autoAlpha: 0, ease: Power2.easeOut, overwrite: true });
    TweenMax.to('.cu-black', 0.5, { autoAlpha: 0, ease: Power2.easeOut, overwrite: true });
    TweenMax.to('.cu-zoom', 0.5, { autoAlpha: 1, ease: Power2.easeOut, overwrite: true });
    TweenMax.to('.particles-wrapper', 0.5, { autoAlpha: 0, ease: Power2.easeOut, overwrite: true });
  };

  var showPlusItem = function() {
    $('.plus-item').show();
    TweenMax.to('.plus-item', 0.5, { autoAlpha: 1, overwrite: true });
  };

  var hidePlusItem = function() {
    TweenMax.to('.plus-item', 0.5, { autoAlpha: 0, overwrite: true });
  };

  var hideAllSections = function() {
    $('.login').hide();
    $('.survey').hide();
    $('.page').hide();
    $('.new').hide();
    $('.ty').hide();
    $('.profile').hide();
    $('.pathfinder').hide();
    $('.underage').hide();
    hideMyIncite();
  };

  var pauseAllMedia = function() {
    removeBg();
    $('[is-audio]').attr('is-audio', 'off');
    audioElement.pause();
  };

  var showMainMenu = function() {
    $('.main-menu').show();
    $('.prev-arrow-btn').hide();
    TweenMax.to('.main-menu', 0.5, { autoAlpha: 1, overwrite: true });
  };

  var hideMainMenu = function() {
    TweenMax.to('.main-menu', 0.5, { autoAlpha: 0, overwrite: true });
  };

  var showNavMenu = function() {
    $('.menu').show();
    TweenMax.to('.menu', 0.5, { autoAlpha: 1, overwrite: true });
  };

  var hideNavMenu = function() {
    TweenMax.to('.menu', 0.5, { autoAlpha: 0, overwrite: true });
  };

  var showNavBack = function() {
    $('.prev-arrow-btn').show();
  };

  var hideNavBack = function() {
    $('.prev-arrow-btn').hide();
  };

  var showMyIncite = function() {
    $('.plus-5').click();
  };

  var hideMyIncite = function() {
    $('.my-incite').hide();
  };

  var showLogin = function() {
    $('.login').show();
    TweenMax.set('.login', { autoAlpha: 0 });
    TweenMax.to('.login', 0.5, { autoAlpha: 1, overwrite: true });
  };

  var showSurvey = function() {
    $('.survey').show();
    TweenMax.set('.survey', { autoAlpha: 0 });
    TweenMax.to('.survey', 0.5, { autoAlpha: 1, overwrite: true });
  };

  var showProfile = function() {
    $('.profile').show();
    TweenMax.set('.profile', { autoAlpha: 0 });
    TweenMax.to('.profile', 0.5, { autoAlpha: 1, overwrite: true });
  };

  var showAbout = function() {
    $('.about').show();
    TweenMax.set('.about', { autoAlpha: 0 });
    TweenMax.to('.about', 0.5, { autoAlpha: 1, overwrite: true });

  };

  var showTransportation = function() {
    $('.transportation').show();
    TweenMax.set('.transportation', { autoAlpha: 0 });
    TweenMax.to('.transportation', 0.5, { autoAlpha: 1, overwrite: true });
  };

  var showMedical = function() {
    $('.medical').show();
    TweenMax.set('.medical', { autoAlpha: 0 });
    TweenMax.to('.medical', 0.5, { autoAlpha: 1, overwrite: true });
  };

  var showVirtual = function() {
    $('.virtual').show();
    TweenMax.set('.virtual', { autoAlpha: 0 });
    TweenMax.to('.virtual', 0.5, { autoAlpha: 1, overwrite: true });
  };

  var showTechnology = function() {
    $('.technology').show();
    TweenMax.set('.technology', { autoAlpha: 0 });
    TweenMax.to('.technology', 0.5, { autoAlpha: 1, overwrite: true });
  };

  var showPathfinder = function() {
    $('.pathfinder').show();
    TweenMax.set('.pathfinder', { autoAlpha: 0 });
    TweenMax.to('.pathfinder', 0.5, { autoAlpha: 1, overwrite: true });
  };

  var hideUnderage = function() {
    TweenMax.to('.underage', 0.5, { autoAlpha: 0, overwrite: true });
  };

  var showUnderage = function() {
    $('.underage').show();
    TweenMax.set('.underage', { autoAlpha: 0 });
    TweenMax.to('.underage', 0.5, { autoAlpha: 1, overwrite: true });
  };

  var hideVideo = function() {
    medicalVideo.pause();
    if (Platform.isIE) {
      TweenMax.set(medicalVideo, { autoAlpha: 0, overwrite: true });
    } else {
      TweenMax.to(medicalVideo, 0.5, { autoAlpha: 0, overwrite: true });
    }
  };

  var showVideo = function() {
    TweenMax.to(medicalVideo, 0.5, { autoAlpha: 1, overwrite: true });
    medicalVideo.currentTime = 0;
    medicalVideo.play();
  };

  var hideTry = function() {
    TweenMax.to('.try', 0.5, { autoAlpha: 0, overwrite: true });
  };

  var showTry = function() {
    TweenMax.to('.try', 0.5, { autoAlpha: 1, overwrite: true });
  };

  var toggleAudio = function(val) {
    var isAudio = $('[is-audio]').attr('is-audio');
    if (val === 'off') {
      $('[is-audio]').attr('is-audio', 'off');
      // audioElement.pause();
      clearAudio();
      return;
    }

    if (isAudio === 'off') {
      $('[is-audio]').attr('is-audio', 'on');
      try {
      audioElement.play();
      } catch(e) {}
    } else {
      $('[is-audio]').attr('is-audio', 'off');
      audioElement.pause();
    }
  };

  var clearAudio = function() {
    audioElement.pause();
    audioElement.setAttribute('src', '');
    // TweenMax.to('.audio-bg1', 1, { autoAlpha: 0 });
    // TweenMax.to('.audio-bg2', 1, { autoAlpha: 0 });
    // TweenMax.to('.audio-bg3', 1, { autoAlpha: 0 });
    hideDials();
    removeBg();
  };

  var hideNextSurvey = function() {
    TweenMax.set('.survey-next', { autoAlpha: 0, overwrite: true });
  };

  var showNextSurvey = function() {
    TweenMax.set('.survey-next', { autoAlpha: 1, overwrite: true });
  };

  // limbic buttons
  TweenMax.to('.dial-ocean, .dial-thunder, .dial-flame', 0.5, { scale: 1, transformOrigin: 'center' });
  var hideDials = function() {
    $('.try-text').show();
    TweenMax.to('.circle-center', 0.5, { scale: 1, autoAlpha: 1, delay: 0.25, transformOrigin: 'center', overwrite: true });
    TweenMax.to('.dial-ocean, .dial-thunder, .dial-flame', 0.5, { scale: 0, overwrite: true });
    TweenMax.to('.try-text', 0.5, { autoAlpha: 1, delay: 0.25, overwrite: true });
    TweenMax.set('.dial-center', { autoAlpha: 0, delay: 0.5 });
    $('.try-center').removeClass('open-dial');
  };

  var showDials = function() {
    $('.try-text').hide();
    TweenMax.to('.circle-center', 0.5, { scale: 2.6, autoAlpha: 0.6, transformOrigin: 'center', overwrite: true });
    TweenMax.to('.dial-ocean, .dial-thunder, .dial-flame', 0.5, { scale: 1, delay: 0.5, overwrite: true });
    TweenMax.set('.try-text', { autoAlpha: 0 });
    TweenMax.set('.dial-center', { autoAlpha: 1 });
    $('.try-center').addClass('open-dial');
  };

  // moods
  var hideMoods = function() {
    TweenMax.to('.moods-list', 0.5, { autoAlpha: 0, overwrite: true });
  };

  var showMoods = function() {
    TweenMax.to('.moods-list', 0.5, { autoAlpha: 1, overwrite: true });
  };

  // notif
  var hideNotif = function() {
    TweenMax.to('.notif-wrapper', 0.5, { autoAlpha: 0, overwrite: true });
  };

  var showNotif = function() {
    TweenMax.to('.notif-wrapper', 0.5, { autoAlpha: 1, overwrite: true });
  };

  // notification
  var hideNotification = function() {
    TweenMax.to('.notification', 0.5, { x: '100%', overwrite: true });
  };

  var showNotification = function() {
    TweenMax.to('.notification', 0.5, { x: '0%', overwrite: true });
  };

  var hideNotifAlert = function() {
    TweenMax.to('.notif-alert', 0.5, { y: '-100%', overwrite: true });
  };

  var showNotifAlert = function() {
    // if (notifAlertClose) return;
    TweenMax.to('.notif-alert', 0.5, { y: '0%', overwrite: true });
  };


  

  





  var showSphere = function() {
    resetSphere();
    showPlusItem();
    hideAllSections();
    pauseAllMedia();
    hideNavMenu();
    hideMainMenu();
    hideNavBack();

    toggleAudio('off');
  };

  var menuBack = function() {
    hideAllSections();
    pauseAllMedia();
    showMainMenu();

    toggleAudio('off');
  };

  var openLogin = function() {
    placeSphereCenterScale();
    hidePlusItem();
    showNavMenu();
    hideNavBack();
    showLogin();
  };

  var openSurvey = function() {
    placeSphereCenterScale();
    hidePlusItem();
    showNavMenu();
    hideNavBack();
    showSurvey();
  };

  var openProfile = function() {
    placeSphereCenterScale();
    hidePlusItem();
    showNavMenu();
    hideNavBack();
    showProfile();
  };

  var openMainMenu = function() {
    showMainMenu();
    placeSphereRight();
    hidePlusItem();
    showNavMenu();
    hideNavBack();
    hideAllSections();
    hideNotification();
    showNotifAlert();
  };

  var openAbout = function() {
    placeSphereCenterScale();
    hidePlusItem();
    showNavMenu();
    hideAllSections();
    hideNavBack();
    showAbout();
    hideNotification();
    showNotifAlert();
  };

  var openTransportation = function() {
    placeSphereRight();
    hidePlusItem();
    showNavMenu();
    hideMainMenu();
    hideAllSections();
    showTransportation();
    showNavBack();
    hideNotification();
    showNotifAlert();
  };

  var openMedical = function() {
    placeSphereRight();
    hidePlusItem();
    showNavMenu();
    hideMainMenu();
    hideAllSections();
    showMedical();
    showNavBack();
    hideNotification();
    showNotifAlert();

    // play video
    showVideo();
    hideDials();

    // hide try
    hideTry();
  };

  var openVirtual = function() {
    placeSphereRight();
    hidePlusItem();
    showNavMenu();
    hideMainMenu();
    hideAllSections();
    showVirtual();
    showNavBack();
    hideNotification();
    showNotifAlert();
  };

  var openTechnology = function() {
    placeSphereRight();
    hidePlusItem();
    showNavMenu();
    hideMainMenu();
    hideAllSections();
    showTechnology();
    showNavBack();
    hideNotification();
    showNotifAlert();
  };

  var openPathfinder = function() {
    placeSphereCenterScale();
    hidePlusItem();
    showNavMenu();
    hideAllSections();
    showPathfinder();
    hideNotification();
    showNotifAlert();
  };

  var openUnderage = function() {
    placeSphereCenterScale();
    hidePlusItem();
    showNavMenu();
    hideAllSections();
    showUnderage();
    hideNotification();
    showNotifAlert();
  };

  // submit form
  window.submitMainForm = function() {
    // submit main form
    $('#main-submit').click();
  };








  /******************
   pages
   ******************/
  var setHome = showSphere;
  var setAbout = openAbout;
  var setTransportation = openTransportation;
  window.setMedical = openMedical;
  var setVirtual = openVirtual;
  var setTechnology = openTechnology;




  /******************
   events
   ******************/
  // advancement plus-0
  $('.plus-0').on('click', function() {
    openMainMenu();
  });

  // my incite plus-5
  $('.plus-5').on('click', function() {
    if (isUserUnderage) {
      openUnderage();
      return;
    }
    if (!isUserLoggedIn) {
      openLogin();
    } else {
      if (isUserCompleted) {
        openProfile();
      } else {
        openSurvey();
      }
    }

    window.location.hash = 'my-incite';
  });
  
  // about plus-1
  $('.plus-1').on('click', function() {
    openAbout();
  });

  // limbic plus-4
  $('.plus-4').on('click', function() {
    openMedical();

    window.location.hash = 'medical';
  });

  // menu
  $('.main-menu-btn').on('click', function() {
    showSphere();
    clearAudio();
  });

  $('.prev-arrow-btn').on('click', function() {
    menuBack();
    clearAudio();
  });

  // my-incite
  $('.view-my-incite').on('click', function() {
    $('.ty').hide();
    openProfile();
  });

  // link
  $('.transpo-link').on('click', function() {
    openTransportation();
  });

  $('.limbic-link').on('click', function() {
    openMedical();
  });

  $('.virtual-link').on('click', function() {
    openVirtual();
  });

  $('.tech-link').on('click', function() {
    openTechnology();
  });

  // audio
  $('.audio-btn').on('click', function(e) {
    showTry();
    hideVideo();
    toggleAudio();
  });

  // video
  $('.icon-cycle').on('click', function(e) {
    hideTry();
    showVideo(0);
    toggleAudio('off');
  });

  // dials
  // limbic buttons
  var isEnable = true;
  $('.try-center').on('click', function(e) {
    if (!isEnable) return;
    showDials();
    isEnable = false;
    setTimeout(function() {
      isEnable = true;
    }, 1100);
  });

  $('.close-center').on('click', function(e) {
    if (!isEnable) return;
    hideDials();
    isEnable = false;
    setTimeout(function() {
      isEnable = true;
    }, 1100);
  });

  // moods
  $('.moods-button').on('click', function(e) {
    showMoods();
  });
  $('.moods-close').on('click', function(e) {
    hideMoods();
  });

  // notif
  $('.deny-btn').on('click', function(e) {
    hideNotif();
    $('#token').val('');
  });
  $('.allow-btn').on('click', function(e) {
    hideNotif();
    initWebPush();
  });

  // notif
  $('.back-to-home').on('click', function(e) {
    hideUnderage();
    showSphere();
  });

  // notif
  $('.notification-close-btn').on('click', function(e) {
    hideNotification();
  });
  $('.notif-alert-close-btn').on('click', function(e) {
    hideNotifAlert();
    notifAlertClose = true;
  });





  /*******************
    cookie functions
    *******************/
  var setCookie = function(cname, cvalue, exdays) {
    var d = new Date();
    d.setTime(d.getTime() + (exdays * 24 * 60 * 60 * 1000));
    var expires = "expires="+d.toUTCString();
    document.cookie = cname + "=" + cvalue + ";" + expires + ";path=/";
  };

  var getCookie = function(cname) {
    var name = cname + "=";
    var ca = document.cookie.split(';');
    for(var i = 0; i < ca.length; i++) {
      var c = ca[i];
      while (c.charAt(0) == ' ') {
        c = c.substring(1);
      }
      if (c.indexOf(name) == 0) {
        return c.substring(name.length, c.length);
      }
    }
    return "";
  };

  var checkCookie = function() {
    var user = getCookie("username");
    if (user != "") {
      alert("Welcome again " + user);
    } else {
      user = prompt("Please enter your name:", "");
      if (user != "" && user != null) {
        setCookie("username", user, 365);
      }
    }
  };

  if (getCookie('isUserUnderage')) {
    isUserUnderage = true;
  }





  /*******************
    utils functions
    *******************/
  var removeBg = function() {
    $('.main-wrapper').removeClass('ocean');
    $('.main-wrapper').removeClass('impulse');
    $('.main-wrapper').removeClass('cozy');
    TweenMax.to('.bg1', 1, { autoAlpha: 0 });
    TweenMax.to('.bg2', 1, { autoAlpha: 0 });
    TweenMax.to('.bg3', 1, { autoAlpha: 0 });
    TweenMax.to('.audio-bg1', 1, { autoAlpha: 0 });
    TweenMax.to('.audio-bg2', 1, { autoAlpha: 0 });
    TweenMax.to('.audio-bg3', 1, { autoAlpha: 0 });
  };

  var audioPlay = function(name) {
    audioElement.setAttribute('src', '');
    setTimeout( function() {

    audioElement.setAttribute('src', '/img/audio/' + name + '.mp3');

    if ( $('[is-audio]').attr('is-audio') === 'off' ) {
      $('.audio-btn').click();
    }
    }, 50);
  };



  var validateForm = function() {
    var valid = true;
    if (
      $('#q1').val() != '' &&
      $('#q2').val() != '' &&
      $('#q3').val() != '' &&
      $('#q4').val() != '' &&
      $('#q5').val() != '' &&
      $('#q6').val() != '' &&
      $('#q7').val() != '' &&
      $('#q8').val() != '' &&
      $('#q9').val() != '' &&
      $('#q10').val() != '' &&
      $('#q11').val() != '' &&
      $('#q12').val() != '' &&
      $('#q13').val() != ''
    ) {
      TweenMax.set('.s-submit', { autoAlpha: 1 });
    } else {
      TweenMax.set('.s-submit', { autoAlpha: 0 });
    }

    validateNext();
  };

  var validateNext = function() {
    if (
      ( $('#q1').val() != '' && currentSurvey === 0 ) ||
      ( $('#q2').val() != '' && currentSurvey === 1 ) ||
      ( $('#q3').val() != '' && currentSurvey === 2 ) ||
      ( $('#q4').val() != '' && currentSurvey === 3 ) ||
      ( $('#q5').val() != '' && currentSurvey === 4 ) ||
      ( $('#q6').val() != '' && currentSurvey === 5 ) ||
      ( $('#q7').val() != '' && currentSurvey === 6 ) ||
      ( $('#q8').val() != '' && currentSurvey === 7 ) ||
      ( $('#q9').val() != '' && currentSurvey === 8 ) ||
      ( $('#q10').val() != '' && currentSurvey === 9 ) ||
      ( $('#q11').val() != '' && currentSurvey === 10 ) ||
      ( $('#q12').val() != '' && currentSurvey === 11 ) ||
      ( $('#q13').val() != '' && currentSurvey === 12 )
      ) {

      if (currentSurvey === 12) {
        hideNextSurvey();
      } else {

        showNextSurvey();
      }

    } else {
      hideNextSurvey();
    }
  }


  var validateEmail = function(email) {
    var re = /^(([^<>()\[\]\\.,;:\s@"]+(\.[^<>()\[\]\\.,;:\s@"]+)*)|(".+"))@((\[[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\])|(([a-zA-Z\-0-9]+\.)+[a-zA-Z]{2,}))$/;
    return re.test(String(email).toLowerCase());
  };

  var validateName = function(name) {
    var regName = /^[a-zA-Z ]+$/ ;
    if (!regName.test(name)) {
        return false;
    } else{
        return true;
    }
  };

  // simple validation on password
  var validatePassword = function(name) {
    if (name.length >= 4) {
        return true;
    } else{
        return false;
    }
  };

  var validateYear = function(year) {
    var text = /^[0-9]+$/;
    var date = new Date().getFullYear();
    if (year.length != 4 || parseInt(year) <= 1900 || date >= date) return false;
    if (!text.test(year)) {
      return false;
    } else {
      return true;
    }
  };

  var validateDob = function(d) {
    var newDate = new Date(d);
    if (d.length != 4) return false;
    if (newDate) return true;
    return false;
  };



  var integrateUserData = function(emailData) {
    /****************************** 
      col 2 = IP
      col 3 = Token
      col 17 = email
      col 18 = fullname
      col 19 = score
    ******************************/
    var tokenCell = _.find(emailData, function(key) { return key.gs$cell.col === '3' });
    var emailCell = _.find(emailData, function(key) { return key.gs$cell.col === '17' });
    var fullnameCell = _.find(emailData, function(key) { return key.gs$cell.col === '18' });
    var scoreCell = _.find(emailData, function(key) { return key.gs$cell.col === '19' });

    var token = (tokenCell ? tokenCell.gs$cell.$t : '');
    var fullname = fullnameCell.gs$cell.$t;
    var email = emailCell.gs$cell.$t;
    var score = scoreCell.gs$cell.$t;

    $('#token').val(token);
    $('#email').val(email);
    $('#fullname').val(fullname);
    $('.user-fullename').text(fullname);
    setScoreData( score );
  };

  var newIntegrateUserData = function(data, myEmail) {
    // var token = data.token;
    var fullname = data.name;
    var email = myEmail;
    var score = data.score;
    var dob = data.dob;

    /*if (!window.isNewToken) {
      $('#token').val(token);
    }*/
    $('#email').val(email);
    $('#fullname').val(fullname);
    $('#dob').val(dob);
    $('.user-fullename').text(fullname.split(' ')[0]);
    setScoreData( score );
  };



  var checkUserComplettion = function(emailData) {
    /****************************** 
      col 4 - 16 = questionaires / answers 
    ******************************/
    var ans1 = _.find(emailData, function(key) { return key.gs$cell.col === '4' });
    var ans2 = _.find(emailData, function(key) { return key.gs$cell.col === '5' });
    var ans3 = _.find(emailData, function(key) { return key.gs$cell.col === '6' });
    var ans4 = _.find(emailData, function(key) { return key.gs$cell.col === '7' });
    var ans5 = _.find(emailData, function(key) { return key.gs$cell.col === '8' });
    var ans6 = _.find(emailData, function(key) { return key.gs$cell.col === '9' });
    var ans7 = _.find(emailData, function(key) { return key.gs$cell.col === '10' });
    var ans8 = _.find(emailData, function(key) { return key.gs$cell.col === '11' });
    var ans9 = _.find(emailData, function(key) { return key.gs$cell.col === '12' });
    var ans10 = _.find(emailData, function(key) { return key.gs$cell.col === '13' });
    var ans11 = _.find(emailData, function(key) { return key.gs$cell.col === '14' });
    var ans12 = _.find(emailData, function(key) { return key.gs$cell.col === '15' });
    var ans13 = _.find(emailData, function(key) { return key.gs$cell.col === '16' });

    if (!ans1 || ans1.content.$t === '') return false;
    if (!ans2 || ans2.content.$t === '') return false;
    if (!ans3 || ans3.content.$t === '') return false;
    if (!ans4 || ans4.content.$t === '') return false;
    if (!ans5 || ans5.content.$t === '') return false;
    if (!ans6 || ans6.content.$t === '') return false;
    if (!ans7 || ans7.content.$t === '') return false;
    if (!ans8 || ans8.content.$t === '') return false;
    if (!ans9 || ans9.content.$t === '') return false;
    if (!ans10 || ans10.content.$t === '') return false;
    if (!ans11 || ans11.content.$t === '') return false;
    if (!ans12 || ans12.content.$t === '') return false;
    if (!ans13 || ans13.content.$t === '') return false;

    return true;
  };


  // calculate score
  var calculateScore = function(emailData) {
    // get total score
    var totalScore = _.reduce($('[score]'), function(memo, num) { 
        return memo + parseInt(num.getAttribute('score')) ;
      }, 0 );

    setScoreData(totalScore);
  };

  // setting score
  var setScoreData = function(totalScore) {
    var scoreData = _.find( INFOGRAPHIC_SCORE, function(num, key, o) { return totalScore <= num.max } );
    if (!scoreData) {
      scoreData = INFOGRAPHIC_SCORE.rebel;
    }

    // set images and texts
    $('.incite-img').attr('src', 'img/profile_infographics/' + scoreData.img);
    $('.result-desc').html(scoreData.desc);
    $('#score').val(totalScore);
  };

  // get age
  var getAge = function(birthDateString) {
    var today = new Date();
    var birthDate = new Date(birthDateString);
    var age = today.getFullYear() - birthDate.getFullYear();
    var m = today.getMonth() - birthDate.getMonth();
    if (m < 0 || (m === 0 && today.getDate() < birthDate.getDate())) {
        age--;
    }
    return age;
  };





  // hash
  var hashes = ['transportation', 'virtual', 'medical', 'technology', 'about', 'my-incite'];
  var hash = window.location.hash.substr(1);

  if (hashes.indexOf(hash) != -1) {
    $('.welcome').hide();
    if (hash === 'technology') {
      setTechnology();
    }
    else if (hash === 'virtual') {
      setVirtual();
    }
    else if (hash === 'medical') {
      setMedical();
    }
    else if (hash === 'transportation') {
      setTransportation();
    }
    else if (hash === 'about') {
      setAbout();
    }
    else if (hash === 'my-incite') {
      showMyIncite();
    }
    else {
      setHome();
    }
  } else {
    $('.welcome').show();
  }

  $('[target-section]').on('click', function() {
    window.location.hash = $(this).attr('target-section');
  });
















  // sign in
  $('.confirmed').hide();
  $('.sign-in').on('click', function() {
    var myEmail = $.trim( $('.login .s-email').val() );
    var myPass = $('.login .s-pass').val();
    var hiddenEmail = $('#email').val();
    var isEmailValid = validateEmail(myEmail);

    $('.login .s-email').removeClass('invalid');
    $('.login .s-pass').removeClass('invalid');
    $('.noemail').hide();

    if (myEmail.length === 0) {
      $('.noemail').show();
      $('.login .s-email').addClass('invalid');
    }

    if (!isEmailValid) {
      $('.login .s-email').addClass('invalid');
    }

    if (!isEmailValid) return;

    $.ajax({
      url: 'https://data-stg.inciteinc.com/email-exists?email=' + myEmail,
    }).done(function( data ) {

      var isEmailInData = data.emailExisting;
      isUserCompleted = data.surveyCompleted && data.score > 0;

      if (isEmailInData) {
        newIntegrateUserData( data, myEmail );
        $('.emailinvalid').hide();
      } else {
        $('.emailinvalid').show();
      }

      if ( (isEmailInData && !isUserCompleted) ) {
        $('.login').addClass('signing-in');
        TweenMax.to('.circle', 1, { rotation: 360, ease: Linear.easeNone, onComplete: function() {
          $('.scan').hide();
          $('.confirmed').show();
          setTimeout(function() {
            $('.login').hide();
            openSurvey();
          }, 500);
        } });

        $('#email').val($('.s-email').val());
        isUserLoggedIn = true;
      } 
      else if ( isEmailInData && isUserCompleted ) {
        $('.login').addClass('signing-in');
        TweenMax.to('.circle', 1, { rotation: 360, ease: Linear.easeNone, onComplete: function() {
          $('.scan').hide();
          $('.confirmed').show();
          setTimeout(function() {
            $('.login').hide();
            $('.profile').show();
            $('.my-incite').show();
          }, 500);
        } });

        isUserLoggedIn = true;
      }

    }); /*end ajax*/

  });

  $('.sign-up').on('click', function() {
    var sfullname = $.trim( $('.new .s-fullname').val() );
    var semail = $.trim( $('.new .s-email').val() );
    var spass1 = $('.new .s-pass1').val();
    var smonth = $('.new .s-month').val();
    var syear = $('.new .s-year').val();
    var sdob = $('.new .s-dob').val();
    var isNameValid = validateName(sfullname);
    var isEmailValid = validateEmail(semail);
    var isPass1Valid = validatePassword(spass1);
    var isDobValid = validateDob(sdob);
    var isAgeOver16 = false;

    $('.new .s-email').removeClass('invalid');
    $('.new .s-dob').removeClass('invalid');
    $('.new .s-fullname').removeClass('invalid');
    $('.new .s-pass1').removeClass('invalid');
    $('.new .s-pass2').removeClass('invalid');

    $('.invalidemail').hide();
    $('.agebelow').hide();
    $('.invalidname').hide();
    $('.invalidpass').hide();
    
    if (!isEmailValid) {
      $('.new .s-email').addClass('invalid');
      $('.invalidemail').show();
    }
    if (!isDobValid) {
      $('.new .s-dob').addClass('invalid');
      $('.agebelow').show();
    }
    if (!isNameValid) {
      $('.new .s-fullname').addClass('invalid');
      $('.invalidname').show();
    }
    if (!isPass1Valid) {
      $('.new .s-pass1').addClass('invalid');
      $('.invalidpass').show();
    }
    
    if (isDobValid && getAge(sdob) < 16) {
      $('.agebelow').show();
      $('.new .s-pas1').addClass('invalid');
      isAgeOver16 = false;
    } else {
      isAgeOver16 = true;
    }

    if (isEmailValid && isNameValid && isPass1Valid && isDobValid && isAgeOver16) {
      $('#fullname').val( sfullname.split(' ')[0] );
      $('#email').val( semail );
      $('#dob').val( sdob );

      $('.new').hide();
      setTimeout(function() {
        openSurvey();
      }, 500);

      submitMainForm();

    }

    if (!isAgeOver16) {
      isUserUnderage = true;
      openUnderage();
      setCookie('isUserUnderage', 1);
    }

  });


  $('.s-submit').on('click', function() {
    // calculate the score first
    calculateScore();

    submitMainForm();

    $('.user-fullename').text( $('.new .s-fullname').val().split(' ')[0] );
    $('.s-submit').hide();
    $('.survey').hide();
    $('.ty').show();
    isUserCompleted = true;

    // notif
    if (!Platform.isIOS && !Platform.isSafari && !Platform.isIE) {
      showNotif();
    }

  });

  $('.cb').on('click', function() {
    var choiceName = $(this).find('input').attr('name');
    var checkedInput = $('input[name="' + choiceName + '"]:checked');
    var forQ = checkedInput.closest('.cb-wrapper').attr('for-q');
    var newVal = $.trim( checkedInput.parent().find('p').text() );
    var scoreValue = checkedInput.attr('score-value');

    checkedInput.closest('[score]').attr('score', scoreValue);

    if (forQ) {
      $('#' + forQ).val(newVal);
      validateForm();
    }
  });

  // fake sign up password
  $('.login .s-email').on('change', function() {
    var isEmailValid = validateEmail( $(this).val() );

    if (isEmailValid) $('.login .s-pass').val('12345678');
  });

  $('.login .s-pass').on('focus', function() {
    var isEmailValid = validateEmail( $('.s-email').val() );
    
    if (isEmailValid) $('.login .s-pass').val('12345678');
  });










  // slider type
  var currentKnob = '.knob1';
  var currentProgress = '.progress1';
  var qs = [1, 4, 5, 6, 12];
  var createDraggable = function(knob, progressBar) {
    // for desktop
    if (winW > MOBILE_BR) {
      // for loop
      for (var i = 0; i < qs.length; i++) {
        var qNum = qs[i];

        TweenMax.set('.q' + qNum + ' .progress', { scaleX: 0, scaleY: 1, transformOrigin: 'top left' });
        TweenMax.set('.q' + qNum + ' .knob', { y: 0 });
        Draggable.create('.q' + qNum + ' .knob', {
          type: 'x', 
          edgeResistance: 1, 
          bounds: '.q' + qNum + ' .draggable-wrapper',
          onDrag: function() {
            var target = this.target;
            var qStr = $(target).closest('[for-q]').attr('for-q');
            var dx = $('.' + qStr + ' .knob')[0]._gsTransform.x + 5;
            var maxX = $('.' + qStr + ' .back').width();
            var one4th = maxX / 4;
            var newProgress = Math.min( 3, parseInt(dx / one4th) );

            // set scale bar
            TweenMax.set('.' + qStr + ' .progress', { scaleX: dx / maxX, transformOrigin: 'top left' });
            $('.' + qStr).attr('progress', newProgress);
            $('.' + qStr).attr('score', (newProgress + 1));

            // get text / input value
            var forQ = $('.' + qStr + ' [for-q]').attr('for-q');
            var newVal = $.trim( $('.' + qStr + ' .opt' + (newProgress + 1))[0].textContent );
            
            if (forQ) {
              $('#' + forQ).val(newVal);
              validateForm();
            }

          }
        });

      }

    } else {
      // for loop
      for (var i = 0; i < qs.length; i++) {
        var qNum = qs[i];

        TweenMax.set('.q' + qNum + ' .progress', { scaleY: 0, scaleX: 1, transformOrigin: 'top left' });
        TweenMax.set('.q' + qNum + ' .knob', { x: 0 });
        Draggable.create('.q' + qNum + ' .knob', {
          type: 'y', 
          edgeResistance: 1, 
          bounds: '.q' + qNum + ' .draggable-wrapper',
          onDrag: function() {
            var target = this.target;
            var qStr = $(target).closest('[for-q]').attr('for-q');
            var dy = $('.' + qStr + ' .knob')[0]._gsTransform.y + 5;
            var maxY = $('.' + qStr + ' .back').height();
            var one4th = maxY / 4;
            var newProgress = Math.min( 3, parseInt(dy / one4th) );

            // set scale bar
            TweenMax.set('.' + qStr + ' .progress', { scaleY: dy / maxY, transformOrigin: 'top left' });
            $('.' + qStr).attr('progress', newProgress);
            $('.' + qStr).attr('score', (newProgress + 1));

            // get text / input value
            var forQ = $('.' + qStr + ' [for-q]').attr('for-q');
            var newVal = $.trim( $('.' + qStr + ' .opt' + (newProgress + 1))[0].textContent );
            
            if (forQ) {
              $('#' + forQ).val(newVal);

              validateForm();
            }

          }
        });

      }

    }

  };





  // 
  $('.box-section .option').on('click', function(e) {
    // because wrapper has score attribute;
    var _this = $(this);
    var wrapper = _this.closest('[score]');
    var forQ = _this.closest('[for-q]').attr('for-q');
    var newVal = _this.text();
    var knob = wrapper.find('.knob');
    var progBar = wrapper.find('.progress');
    var qStr = parseInt( _this.attr('class').substr(-1, 1) );
    var max;
    var one4th;
    var d0;
    var d1;
    var prog;

    if (winW > MOBILE_BR) {
      max = wrapper.find('.draggable-wrapper').width();
      one4th = max / 4;

      d0 = one4th * qStr;
      d1 = d0 - (one4th / 2);
      prog = qStr - 1;
      
      wrapper.attr('score', qStr);
      wrapper.attr('progress', prog);
      TweenMax.set(knob, { x: d1 });
      TweenMax.set(progBar, { scaleX: d1 / max, transformOrigin: 'top left' });

      if (forQ) {
        $('#' + forQ).val(newVal);
        validateForm();
      }
    } else {
      max = wrapper.find('.draggable-wrapper').height();
      one4th = max / 4;

      d0 = one4th * qStr;
      d1 = d0 - (one4th / 2);
      prog = qStr - 1;
      
      wrapper.attr('score', qStr);
      wrapper.attr('progress', prog);
      TweenMax.set(knob, { y: d1 });
      TweenMax.set(progBar, { scaleY: d1 / max, transformOrigin: 'top left' });

      if (forQ) {
        $('#' + forQ).val(newVal);
        validateForm();
      }
    }

  });


  $('#datepicker').datepicker({ 
      dateFormat: 'yy/mm',
      changeMonth: true,
      changeYear: true,
      showButtonPanel: true,
      yearRange: '1950:2020',
      defaultDate: new Date(),
      onClose: function(dateText, inst) { 
        // $('#datepicker').val( inst.selectedYear + '/' + ('0' + (inst.selectedMonth + 1)).slice(-2) );
        $('#datepicker').val( inst.selectedYear );
      }
  });




  // gallery
  $('[target-gallery]').on('click', function() {
    var newGallery = $(this).attr('target-gallery');
    $(this).closest('[selected-gallery]').attr('selected-gallery', newGallery);
  });




  // limbic dials
  $('.dial-ocean').on('click', function(e) {
    removeBg();
    $('.main-wrapper').addClass('ocean');
    TweenMax.to('.bg1', 1, { autoAlpha: 1 });
    TweenMax.to('.audio-bg1', 1, { autoAlpha: 1 });
    
    hideVideo();
    audioPlay('ocean');
  });

  $('.dial-thunder').on('click', function(e) {
    removeBg();
    $('.main-wrapper').addClass('impulse');
    TweenMax.to('.bg2', 1, { autoAlpha: 1 });
    TweenMax.to('.audio-bg2', 1, { autoAlpha: 1 });
    
    hideVideo();
    audioPlay('impulse');
  });

  $('.dial-flame').on('click', function(e) {
    removeBg();
    $('.main-wrapper').addClass('cozy');
    TweenMax.to('.bg3', 1, { autoAlpha: 1 });
    TweenMax.to('.audio-bg3', 1, { autoAlpha: 1 });
    
    hideVideo();
    audioPlay('cozy');
  });

  $('.explore-btn').on('click', function(e) {
    var target = $(this).attr('target-id');
    var top = $('.main-wrapper').scrollTop() + $('#' + target).offset().top;
    $('.main-wrapper').animate({
      scrollTop: top
    }, 500);
  });




  // survey buttons
  $('.ty-close-btn').on('click', function(e) {
    $('.ty').hide();
    $('.profile').show();
    $('.my-incite').show();
  });

  $('.result-close-btn').on('click', function(e) {
    $('.pathfinder').addClass('show-all');
  });

  $('.create-an-account').on('click', function(e) {
    $('.login').hide();
    $('.new').show();
  });

  $('.login-here').on('click', function(e) {
    $('.login').show();
    $('.new').hide();
  });

  TweenMax.set('.survey-prev', { autoAlpha: 0 });
  TweenMax.set('.survey-next', { autoAlpha: 0 });
  $('.survey-prev').on('click', function(e) {
    currentSurvey --;

    if (currentSurvey <= 0) {
      currentSurvey = 0;
      TweenMax.set('.survey-prev', { autoAlpha: 0 });
    } else {
      TweenMax.set('.survey-prev', { autoAlpha: 1 });
      TweenMax.set('.survey-next', { autoAlpha: 1 });
    }

    TweenMax.to('.survey-box', 0.5, { x: (-100 * currentSurvey) + '%', ease: Power2.easeOut });

  });

  $('.survey-next').on('click', function(e) {
    currentSurvey ++;

    if (currentSurvey >= 12) {
      currentSurvey = 12;
      TweenMax.set('.survey-next', { autoAlpha: 0 });
    } else {
      TweenMax.set('.survey-next', { autoAlpha: 1 });
      TweenMax.set('.survey-prev', { autoAlpha: 1 });
    }

    TweenMax.to('.survey-box', 0.5, { x: (-100 * currentSurvey) + '%', ease: Power2.easeOut });

    validateNext();
  });



  
  // infinite loop
  audioElement.addEventListener('ended', function() {
    this.play();
  }, false);

  // auto play on start
  audioElement.addEventListener('loadstart', function() {
    setTimeout(function() {
      try {
      audioElement.play();
      } catch(e) {}
    }, 100);
  }, false);

  medicalVideo.addEventListener('ended', function() {
    hideVideo();
    showTry();
  });

  // particles video
  $('.vid1')[0].addEventListener('playing', function() {
    // console.log('vid1 playing')
  });
  $('.vid2')[0].addEventListener('playing', function() {
    // console.log('vid2 playing')
  });
  $('.vid3')[0].addEventListener('playing', function() {
    // console.log('vid3 playing')
  });

  var stateTicker = setInterval(function() {
    // if ($('.vid1')[0].readyState === 4 && $('.vid2')[0].readyState === 4 && $('.vid3')[0].readyState === 4) {
    if ($('.vid3')[0].readyState === 4) {
      introTL.play(0);
      $('.spinner').remove();
      clearInterval(stateTicker);

      setTimeout(autoPlayOtherVideo, 1000);
    }
  }, 100);
  // introTL.play(0);

  var autoPlayOtherVideo = function() {
    $('.vid1')[0].play();
    $('.vid2')[0].play();
    $('.vid4')[0].play();
    $('.particles-video')[0].play();
  };





  // sound wave animation is buggy when on IE.
  var tlWave = new TimelineMax({ onComplete: function() {
    this.restart();
  } });
  tlWave.to('.wave-line', 2, { x: '-70%', ease: Linear.easeNone });
  tlWave.to('.wave-line', 2, { x: '0%', ease: Linear.easeNone });
    






  // fix resize
  var blocker = setInterval(function() {
    $('.blocker').css('height', 0);
    $('.footer').css('top', 0);
    $('.blocker').css('height', $('.main-wrapper')[0].scrollHeight);
    $('.footer').css('top', $('.main-wrapper')[0].scrollHeight - 20 - $('.footer').height());
  }, 1000 / 18);
    






  // fix resize
  var fixResize = function() {
    winW = window.innerWidth;
    winH = window.innerHeight;

    resizeFakeShape();
    createDraggable(currentKnob, currentProgress);

    if (winW <= MOBILE_BR && isMenuAnimate) {
      tl.play(0);
    }
  };

  $(window).resize(fixResize);
  fixResize();


});



