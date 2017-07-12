using System.ComponentModel;
using System.Windows.Forms;

namespace BritePacketBuilder
{
    partial class MainForm
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(MainForm));
            this.stsMain = new System.Windows.Forms.StatusStrip();
            this.lblStatus = new System.Windows.Forms.ToolStripStatusLabel();
            this.tlsMain = new System.Windows.Forms.ToolStrip();
            this.btnFile = new System.Windows.Forms.ToolStripDropDownButton();
            this.btnOpenFolder = new System.Windows.Forms.ToolStripMenuItem();
            this.btnNew = new System.Windows.Forms.ToolStripMenuItem();
            this.btnSave = new System.Windows.Forms.ToolStripMenuItem();
            this.btnSaveAs = new System.Windows.Forms.ToolStripMenuItem();
            this.btnExit = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
            this.lblPort = new System.Windows.Forms.ToolStripLabel();
            this.cmbPorts = new System.Windows.Forms.ToolStripComboBox();
            this.lblBaudRate = new System.Windows.Forms.ToolStripLabel();
            this.cmbBaudRates = new System.Windows.Forms.ToolStripComboBox();
            this.btnConnect = new System.Windows.Forms.ToolStripButton();
            this.splMain = new System.Windows.Forms.SplitContainer();
            this.splPackets = new System.Windows.Forms.SplitContainer();
            this.grpPacketFiles = new System.Windows.Forms.GroupBox();
            this.tvFiles = new System.Windows.Forms.TreeView();
            this.splPacket = new System.Windows.Forms.SplitContainer();
            this.grpPacketBuild = new System.Windows.Forms.GroupBox();
            this.btnSend = new System.Windows.Forms.Button();
            this.txtPacket = new System.Windows.Forms.TextBox();
            this.grpRemoteData = new System.Windows.Forms.GroupBox();
            this.txtRemoteData = new System.Windows.Forms.TextBox();
            this.txtLog = new System.Windows.Forms.TextBox();
            this.stsMain.SuspendLayout();
            this.tlsMain.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.splMain)).BeginInit();
            this.splMain.Panel1.SuspendLayout();
            this.splMain.Panel2.SuspendLayout();
            this.splMain.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.splPackets)).BeginInit();
            this.splPackets.Panel1.SuspendLayout();
            this.splPackets.Panel2.SuspendLayout();
            this.splPackets.SuspendLayout();
            this.grpPacketFiles.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.splPacket)).BeginInit();
            this.splPacket.Panel1.SuspendLayout();
            this.splPacket.Panel2.SuspendLayout();
            this.splPacket.SuspendLayout();
            this.grpPacketBuild.SuspendLayout();
            this.grpRemoteData.SuspendLayout();
            this.SuspendLayout();
            // 
            // stsMain
            // 
            this.stsMain.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.lblStatus});
            this.stsMain.Location = new System.Drawing.Point(0, 615);
            this.stsMain.Name = "stsMain";
            this.stsMain.Size = new System.Drawing.Size(999, 22);
            this.stsMain.TabIndex = 1;
            // 
            // lblStatus
            // 
            this.lblStatus.Name = "lblStatus";
            this.lblStatus.Size = new System.Drawing.Size(79, 17);
            this.lblStatus.Text = "Disconnected";
            // 
            // tlsMain
            // 
            this.tlsMain.GripStyle = System.Windows.Forms.ToolStripGripStyle.Hidden;
            this.tlsMain.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.btnFile,
            this.toolStripSeparator1,
            this.lblPort,
            this.cmbPorts,
            this.lblBaudRate,
            this.cmbBaudRates,
            this.btnConnect});
            this.tlsMain.Location = new System.Drawing.Point(0, 0);
            this.tlsMain.Name = "tlsMain";
            this.tlsMain.RenderMode = System.Windows.Forms.ToolStripRenderMode.System;
            this.tlsMain.Size = new System.Drawing.Size(999, 25);
            this.tlsMain.TabIndex = 2;
            this.tlsMain.Text = "toolStrip1";
            // 
            // btnFile
            // 
            this.btnFile.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
            this.btnFile.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.btnOpenFolder,
            this.btnNew,
            this.btnSave,
            this.btnSaveAs,
            this.btnExit});
            this.btnFile.Image = ((System.Drawing.Image)(resources.GetObject("btnFile.Image")));
            this.btnFile.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.btnFile.Name = "btnFile";
            this.btnFile.Size = new System.Drawing.Size(38, 22);
            this.btnFile.Text = "File";
            // 
            // btnOpenFolder
            // 
            this.btnOpenFolder.Name = "btnOpenFolder";
            this.btnOpenFolder.Size = new System.Drawing.Size(139, 22);
            this.btnOpenFolder.Text = "Open Folder";
            this.btnOpenFolder.Click += new System.EventHandler(this.btnOpenFolder_Click);
            // 
            // btnNew
            // 
            this.btnNew.Name = "btnNew";
            this.btnNew.Size = new System.Drawing.Size(139, 22);
            this.btnNew.Text = "New";
            this.btnNew.Click += new System.EventHandler(this.btnNew_Click);
            // 
            // btnSave
            // 
            this.btnSave.Name = "btnSave";
            this.btnSave.Size = new System.Drawing.Size(139, 22);
            this.btnSave.Text = "Save";
            this.btnSave.Click += new System.EventHandler(this.btnSave_Click);
            // 
            // btnSaveAs
            // 
            this.btnSaveAs.Name = "btnSaveAs";
            this.btnSaveAs.Size = new System.Drawing.Size(139, 22);
            this.btnSaveAs.Text = "Save As";
            this.btnSaveAs.Click += new System.EventHandler(this.btnSaveAs_Click);
            // 
            // btnExit
            // 
            this.btnExit.Name = "btnExit";
            this.btnExit.Size = new System.Drawing.Size(139, 22);
            this.btnExit.Text = "Exit";
            this.btnExit.Click += new System.EventHandler(this.btnExit_Click);
            // 
            // toolStripSeparator1
            // 
            this.toolStripSeparator1.Name = "toolStripSeparator1";
            this.toolStripSeparator1.Size = new System.Drawing.Size(6, 25);
            // 
            // lblPort
            // 
            this.lblPort.Name = "lblPort";
            this.lblPort.Size = new System.Drawing.Size(29, 22);
            this.lblPort.Text = "Port";
            // 
            // cmbPorts
            // 
            this.cmbPorts.Name = "cmbPorts";
            this.cmbPorts.Size = new System.Drawing.Size(121, 25);
            this.cmbPorts.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.cmbPorts_KeyPress);
            // 
            // lblBaudRate
            // 
            this.lblBaudRate.Name = "lblBaudRate";
            this.lblBaudRate.Size = new System.Drawing.Size(60, 22);
            this.lblBaudRate.Text = "Baud Rate";
            // 
            // cmbBaudRates
            // 
            this.cmbBaudRates.Items.AddRange(new object[] {
            "300",
            "600",
            "1200",
            "2400",
            "4800",
            "9600",
            "14400",
            "19200",
            "28800",
            "38400",
            "57600",
            "115200"});
            this.cmbBaudRates.Name = "cmbBaudRates";
            this.cmbBaudRates.Size = new System.Drawing.Size(121, 25);
            this.cmbBaudRates.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.cmbBaudRates_KeyPress);
            // 
            // btnConnect
            // 
            this.btnConnect.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
            this.btnConnect.Image = ((System.Drawing.Image)(resources.GetObject("btnConnect.Image")));
            this.btnConnect.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.btnConnect.Name = "btnConnect";
            this.btnConnect.Size = new System.Drawing.Size(56, 22);
            this.btnConnect.Text = "Connect";
            this.btnConnect.Click += new System.EventHandler(this.btnConnect_Click);
            // 
            // splMain
            // 
            this.splMain.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splMain.Location = new System.Drawing.Point(0, 25);
            this.splMain.Name = "splMain";
            this.splMain.Orientation = System.Windows.Forms.Orientation.Horizontal;
            // 
            // splMain.Panel1
            // 
            this.splMain.Panel1.Controls.Add(this.splPackets);
            // 
            // splMain.Panel2
            // 
            this.splMain.Panel2.Controls.Add(this.txtLog);
            this.splMain.Size = new System.Drawing.Size(999, 590);
            this.splMain.SplitterDistance = 435;
            this.splMain.TabIndex = 3;
            // 
            // splPackets
            // 
            this.splPackets.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splPackets.Location = new System.Drawing.Point(0, 0);
            this.splPackets.Name = "splPackets";
            // 
            // splPackets.Panel1
            // 
            this.splPackets.Panel1.Controls.Add(this.grpPacketFiles);
            // 
            // splPackets.Panel2
            // 
            this.splPackets.Panel2.Controls.Add(this.splPacket);
            this.splPackets.Size = new System.Drawing.Size(999, 435);
            this.splPackets.SplitterDistance = 236;
            this.splPackets.TabIndex = 0;
            // 
            // grpPacketFiles
            // 
            this.grpPacketFiles.Controls.Add(this.tvFiles);
            this.grpPacketFiles.Dock = System.Windows.Forms.DockStyle.Fill;
            this.grpPacketFiles.Location = new System.Drawing.Point(0, 0);
            this.grpPacketFiles.Name = "grpPacketFiles";
            this.grpPacketFiles.Size = new System.Drawing.Size(236, 435);
            this.grpPacketFiles.TabIndex = 0;
            this.grpPacketFiles.TabStop = false;
            this.grpPacketFiles.Text = "Packet Files";
            // 
            // tvFiles
            // 
            this.tvFiles.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tvFiles.Location = new System.Drawing.Point(3, 16);
            this.tvFiles.Name = "tvFiles";
            this.tvFiles.Size = new System.Drawing.Size(230, 416);
            this.tvFiles.TabIndex = 1;
            this.tvFiles.NodeMouseDoubleClick += new System.Windows.Forms.TreeNodeMouseClickEventHandler(this.tvFiles_NodeMouseDoubleClick);
            // 
            // splPacket
            // 
            this.splPacket.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splPacket.Location = new System.Drawing.Point(0, 0);
            this.splPacket.Name = "splPacket";
            this.splPacket.Orientation = System.Windows.Forms.Orientation.Horizontal;
            // 
            // splPacket.Panel1
            // 
            this.splPacket.Panel1.Controls.Add(this.grpPacketBuild);
            // 
            // splPacket.Panel2
            // 
            this.splPacket.Panel2.Controls.Add(this.grpRemoteData);
            this.splPacket.Size = new System.Drawing.Size(759, 435);
            this.splPacket.SplitterDistance = 216;
            this.splPacket.TabIndex = 0;
            // 
            // grpPacketBuild
            // 
            this.grpPacketBuild.Controls.Add(this.btnSend);
            this.grpPacketBuild.Controls.Add(this.txtPacket);
            this.grpPacketBuild.Dock = System.Windows.Forms.DockStyle.Fill;
            this.grpPacketBuild.Location = new System.Drawing.Point(0, 0);
            this.grpPacketBuild.Name = "grpPacketBuild";
            this.grpPacketBuild.Size = new System.Drawing.Size(759, 216);
            this.grpPacketBuild.TabIndex = 1;
            this.grpPacketBuild.TabStop = false;
            this.grpPacketBuild.Text = "Build Packet";
            // 
            // btnSend
            // 
            this.btnSend.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.btnSend.Location = new System.Drawing.Point(678, 187);
            this.btnSend.Name = "btnSend";
            this.btnSend.Size = new System.Drawing.Size(75, 23);
            this.btnSend.TabIndex = 4;
            this.btnSend.Text = "Send";
            this.btnSend.UseVisualStyleBackColor = true;
            this.btnSend.Click += new System.EventHandler(this.btnSend_Click);
            // 
            // txtPacket
            // 
            this.txtPacket.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.txtPacket.BackColor = System.Drawing.SystemColors.Window;
            this.txtPacket.Font = new System.Drawing.Font("Consolas", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.txtPacket.Location = new System.Drawing.Point(3, 16);
            this.txtPacket.Multiline = true;
            this.txtPacket.Name = "txtPacket";
            this.txtPacket.Size = new System.Drawing.Size(750, 165);
            this.txtPacket.TabIndex = 2;
            this.txtPacket.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.txtPacket_KeyPress);
            // 
            // grpRemoteData
            // 
            this.grpRemoteData.Controls.Add(this.txtRemoteData);
            this.grpRemoteData.Dock = System.Windows.Forms.DockStyle.Fill;
            this.grpRemoteData.Location = new System.Drawing.Point(0, 0);
            this.grpRemoteData.Name = "grpRemoteData";
            this.grpRemoteData.Size = new System.Drawing.Size(759, 215);
            this.grpRemoteData.TabIndex = 0;
            this.grpRemoteData.TabStop = false;
            this.grpRemoteData.Text = "Remote Data";
            // 
            // txtRemoteData
            // 
            this.txtRemoteData.BackColor = System.Drawing.SystemColors.Window;
            this.txtRemoteData.Dock = System.Windows.Forms.DockStyle.Fill;
            this.txtRemoteData.Font = new System.Drawing.Font("Consolas", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.txtRemoteData.Location = new System.Drawing.Point(3, 16);
            this.txtRemoteData.Multiline = true;
            this.txtRemoteData.Name = "txtRemoteData";
            this.txtRemoteData.ReadOnly = true;
            this.txtRemoteData.Size = new System.Drawing.Size(753, 196);
            this.txtRemoteData.TabIndex = 2;
            // 
            // txtLog
            // 
            this.txtLog.BackColor = System.Drawing.SystemColors.Window;
            this.txtLog.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.txtLog.Dock = System.Windows.Forms.DockStyle.Fill;
            this.txtLog.Font = new System.Drawing.Font("Consolas", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.txtLog.Location = new System.Drawing.Point(0, 0);
            this.txtLog.Multiline = true;
            this.txtLog.Name = "txtLog";
            this.txtLog.ReadOnly = true;
            this.txtLog.Size = new System.Drawing.Size(999, 151);
            this.txtLog.TabIndex = 0;
            // 
            // MainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(999, 637);
            this.Controls.Add(this.splMain);
            this.Controls.Add(this.tlsMain);
            this.Controls.Add(this.stsMain);
            this.Name = "MainForm";
            this.ShowIcon = false;
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Brite Packet Builder";
            this.Load += new System.EventHandler(this.MainForm_Load);
            this.stsMain.ResumeLayout(false);
            this.stsMain.PerformLayout();
            this.tlsMain.ResumeLayout(false);
            this.tlsMain.PerformLayout();
            this.splMain.Panel1.ResumeLayout(false);
            this.splMain.Panel2.ResumeLayout(false);
            this.splMain.Panel2.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.splMain)).EndInit();
            this.splMain.ResumeLayout(false);
            this.splPackets.Panel1.ResumeLayout(false);
            this.splPackets.Panel2.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.splPackets)).EndInit();
            this.splPackets.ResumeLayout(false);
            this.grpPacketFiles.ResumeLayout(false);
            this.splPacket.Panel1.ResumeLayout(false);
            this.splPacket.Panel2.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.splPacket)).EndInit();
            this.splPacket.ResumeLayout(false);
            this.grpPacketBuild.ResumeLayout(false);
            this.grpPacketBuild.PerformLayout();
            this.grpRemoteData.ResumeLayout(false);
            this.grpRemoteData.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private StatusStrip stsMain;
        private ToolStripStatusLabel lblStatus;
        private ToolStrip tlsMain;
        private ToolStripDropDownButton btnFile;
        private ToolStripMenuItem btnNew;
        private ToolStripMenuItem btnOpenFolder;
        private ToolStripMenuItem btnSave;
        private ToolStripMenuItem btnSaveAs;
        private ToolStripMenuItem btnExit;
        private ToolStripSeparator toolStripSeparator1;
        private ToolStripComboBox cmbPorts;
        private ToolStripButton btnConnect;
        private SplitContainer splMain;
        private TextBox txtLog;
        private SplitContainer splPackets;
        private SplitContainer splPacket;
        private GroupBox grpPacketFiles;
        private TreeView tvFiles;
        private GroupBox grpRemoteData;
        private GroupBox grpPacketBuild;
        private TextBox txtRemoteData;
        private TextBox txtPacket;
        private Button btnSend;
        private ToolStripLabel lblPort;
        private ToolStripLabel lblBaudRate;
        private ToolStripComboBox cmbBaudRates;
    }
}

