/**
 * Get the controller object attached to this element.
 * Exists as a workaround for the inability to subclass Element.
 */
function ctrl(element) {
    return element.ctrl;
}

function element(ctrl) {
    return ctrl.element;
}

function attach(element, ctrl) {
    element.ctrl = ctrl;
    ctrl.element = element;
}

function now() {
    return new Date().getTime();
}

function prefix(s, c, n) {
    while (s.length < n) {
        s = c + s;
    }
    return s;
}
