{
  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
  };

  outputs = { self, nixpkgs }:
    let
      concord-dev = import ./concord.nix {
        inherit (pkgs) lib stdenv fetchFromGitHub curl;
      };
      system = "x86_64-linux";
      pkgs = nixpkgs.legacyPackages.${system};
      fhs = pkgs.buildFHSEnv {
        name = "fhs-shell";
        targetPkgs = pkgs: with pkgs; [
          gnumake gcc curl curl.dev concord-dev
        ];
      };
    in
      {
        devShells.${system}.default = fhs.env;
      };
}
