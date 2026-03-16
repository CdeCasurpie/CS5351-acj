{
  description = "Entorno de desarrollo reproducible para Tesis (C++ & Python)";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
  };

  outputs = { self, nixpkgs }:
  let
    system = "x86_64-linux";
    pkgs = import nixpkgs { inherit system; };
  in {
    devShells.${system}.default = pkgs.mkShell {
      name = "tesis-acj-env";

      buildInputs = with pkgs; [
        cmake
        gcc
        gnumake
        cgal
        boost
		python3
        python3Packages.pip
        python3Packages.virtualenv
		libsForQt5.qtbase
        libGL
        libGLU
        xorg.libX11
        stdenv.cc.cc.lib
      ];

      shellHook = ''
        echo "Entorno NixOS"
        
        export LD_LIBRARY_PATH="${pkgs.libGL}/lib:${pkgs.libGLU}/lib:${pkgs.xorg.libX11}/lib:${pkgs.stdenv.cc.cc.lib}/lib:$LD_LIBRARY_PATH"
        export QT_QPA_PLATFORM_PLUGIN_PATH="${pkgs.libsForQt5.qtbase.bin}/lib/qt-${pkgs.libsForQt5.qtbase.version}/plugins"

        if [ ! -d ".venv" ]; then
          echo "📦 Creando entorno virtual local..."
          python -m venv .venv
        fi
        
        source .venv/bin/activate
      '';
    };
  };
}
