{
  lib,
  stdenv,
  fetchFromGitHub,
  curl,
}:

stdenv.mkDerivation (finalAttrs: {
  pname = "concord";
  version = "2498dd2e1762141c0faf27a6e17a3e32e9117025";

  src = fetchFromGitHub {
    owner = "Cogmasters";
    repo = "concord";
    rev = "${finalAttrs.version}";
    hash = "sha256-yOLBoKNOti9S7ofEvfFZDPOI9IqC2n5CAGElULnw5Rw=";
  };

  makeFlags = [ "PREFIX=${placeholder "out"}" "PKGCONFIGDIR=${placeholder "out"}" ];

  buildInputs = [ curl ];

  meta = {
    description = "Discord API wrapper library made in C";
    homepage = "https://cogmasters.github.io/concord/";
    license = lib.licenses.mit;
    maintainers = with lib.maintainers; [ sigmanificient ];
    platforms = lib.platforms.unix;
  };
})
